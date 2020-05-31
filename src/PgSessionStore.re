[@bs.deriving abstract]
type options = {
  pool: Pg.pool,
  [@bs.optional]
  tableName: string,
};

let make: (ExpressSession.t, options) => ExpressSession.store = [%raw
  {|
  (session, options) => {
    let pgSimple = require("connect-pg-simple")
    let Store = pgSimple(session)
    return new Store(options)
  }
  |}
];