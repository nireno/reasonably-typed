[@bs.deriving abstract]
type options = {logger: Pino.t};

[@bs.module]
external make : options => Express.Middleware.t =
  "express-pino-logger";
