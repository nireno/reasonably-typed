type pool;
type row = Js.Json.t;

type result = {
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

let query: (pool, string) => Js.Promise.t(result) = [%bs.raw
  {| function(pool, sql) {
    return pool.query(sql);
  }|}
];

let query2: (pool, string, array(string)) => Js.Promise.t(result) = [%bs.raw
  {| function(pool, sql, options) {
    return pool.query(sql, options);
  }|}
];

let endPool: pool => unit = [%bs.raw {|function(pool){ pool.end();}|}];
