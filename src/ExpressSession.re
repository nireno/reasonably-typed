[@bs.deriving abstract]
type cookieOptions = {
  secure: bool,
  [@bs.optional]
  maxAge: int,
};

type store;

[@bs.deriving abstract]
type options = {
  [@bs.optional]
  store,
  secret: string,
  resave: bool,
  saveUninitialized: bool,
  cookie: cookieOptions,
};

type t = options => Express.Middleware.t;

[@bs.module] external session: t = "express-session";

[@bs.get] external asJson: Express.Request.t => Js.Json.t = "";
[@bs.get] external sessionID: Express.Request.t => string = "";

let set: (Express.Request.t, string, 'a) => unit =
  [@ocaml.warning "-27"]
  ((req, key, value) => [%raw "req.session[key] = value"]);

let get: (Express.Request.t, string) => 'a =
  [@ocaml.warning "-27"] ((req, key) => [%raw "req.session[key]"]);

type saveError;
type saveResult;

[@bs.send] [@bs.scope "session"]
external save: (Express.Request.t, (saveError, saveResult) => unit) => unit =
  "save";

let save = NodeUtil.promisify1(save);
