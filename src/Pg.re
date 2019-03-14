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

let rawRequest: (pool, string) => Js.Promise.t(response) = [%bs.raw
  {| function(pool, sql) {
    return pool.query(sql);
  }|}
];

let request: (pool, string, array(string)) => Js.Promise.t(response) = [%bs.raw
  {| function(pool, sql, options) {
    return pool.query(sql, options);
  }|}
];

let rawQuery: (pool, string) => Js.Promise.t(array(row)) =
  (pool, sql) =>
    rawRequest(pool, sql)
    |> Js.Promise.then_(response => response##rows |> Js.Promise.resolve);

let query: (pool, string, array(string)) => Js.Promise.t(array(row)) =
  (pool, sql, params) =>
    request(pool, sql, params)
    |> Js.Promise.then_(response => response##rows |> Js.Promise.resolve);

let rawQueryOne: (pool, string) => Js.Promise.t(option(row)) =
  (pool, sql) =>
    rawQuery(pool, sql)
    |> Js.Promise.then_(rows =>
         (
           switch (rows) {
           | [||] => None
           | rows => Some(rows[0])
           }
         )
         |> Js.Promise.resolve
       );

let queryOne: (pool, string, array(string)) => Js.Promise.t(option(row)) =
  (pool, sql, params) =>
    query(pool, sql, params)
    |> Js.Promise.then_(rows =>
         Js.Promise.resolve(
           switch (rows) {
           | [||] => None
           | rows => Some(rows[0])
           },
         )
       );

let endPool: pool => unit = [%bs.raw {|function(pool){ pool.end();}|}];
