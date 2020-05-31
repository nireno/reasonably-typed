type pool;
type row = Js.Json.t;

type response = {
  .
  "rows": array(row),
  "rowCount": int,
};

type poolConfig = {
  .
  "user": string,
  "host": string,
  "database": string,
  "password": string,
  "port": int,
};

type param;

type queryConfig = {
  .
  "text": string,
  "values": array(param),
  "rowMode": string,
};

[@bs.module "pg"] [@bs.new] external makePool_: poolConfig => pool = "Pool";

let makePool =
    (~database, ~user, ~password, ~host="localhost", ~port=5432, ()) =>
  makePool_({
    "database": database,
    "user": user,
    "password": password,
    "host": host,
    "port": port,
  });

[@bs.send] external query: (pool, queryConfig) => Js.Promise.t(response) = "query";
[@bs.send] external endPool: pool => Js.Promise.t(response) = "end";

let request = (pool, text, ~values=[||], ~arrayMode=false, ()) => {
  let values': array(param) = Array.map(x => Obj.magic(x), values);
  let queryConfig = {
    "text": text,
    "values": values',
    "rowMode": arrayMode ? "array" : "",
  };
  query(pool, queryConfig);
};

let query = (pool, text, ~values=[||], ~arrayMode=false, ()) =>
  request(pool, text, ~values, ~arrayMode, ())
  |> Js.Promise.then_(response => response##rows |> Js.Promise.resolve);

let queryOne = (pool, text, ~values=[||], ~arrayMode=false, ()) =>
  query(pool, text, ~values, ~arrayMode, ())
  |> Js.Promise.then_(rows =>
       (
         switch (rows) {
         | [||] => None
         | rows => Some(rows[0])
         }
       )
       |> Js.Promise.resolve
     );

let queryOneValue:
  (
    pool,
    string,
    ~values: array('a)=?,
    ~decoder: Js.Json.t => option('a),
    unit
  ) =>
  Js.Promise.t(option('a)) =
  (pool, text, ~values=[||], ~decoder, ()) => {
    queryOne(pool, text, ~values, ~arrayMode=true, ())
    |> Js.Promise.then_(maybeRow =>
         (
           switch (maybeRow) {
           | None => None
           | Some(row) =>
             let xs: array(Js.Json.t) = Obj.magic(row);
             switch (xs) {
             | [||] => None
             | arr => arr[0] |> decoder
             };
           }
         )
         |> Js.Promise.resolve
       );
  };

let queryOneString = queryOneValue(~decoder=Js.Json.decodeString);

module type PgPool = {let pool: pool;};

module Make = (Pool: PgPool) => {
  let request = (text, ~values=[||], ~arrayMode=false, ()) =>
    request(Pool.pool, text, ~values, ~arrayMode, ());

  let query = (text, ~values=[||], ~arrayMode=false, ()) =>
    query(Pool.pool, text, ~values, ~arrayMode, ());

  let queryOne = (text, ~values=[||], ~arrayMode=false, ()) =>
    queryOne(Pool.pool, text, ~values, ~arrayMode, ());

  let queryOneValue = (text, ~values=[||], ~decoder, ()) =>
    queryOneValue(Pool.pool, text, ~values, ~decoder, ());

  let queryOneString = (text, ~values=[||], ()) =>
    queryOneString(Pool.pool, text, ~values, ());

  let endPool = () => endPool(Pool.pool);
};
