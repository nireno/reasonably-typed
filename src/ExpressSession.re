[@bs.deriving abstract]
type cookieOptions = {
  secure: bool,
  [@bs.optional]
  maxAge: int,
};

[@bs.deriving abstract]
type options = {
  secret: string,
  resave: bool,
  saveUninitialized: bool,
  cookie: cookieOptions,
};

[@bs.module]
external make: options => Express.Middleware.t = "express-session";

[@bs.get] external session: Express.Request.t => Js.Json.t = "";
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
