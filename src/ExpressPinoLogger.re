[@bs.deriving abstract]
type options = {pino: Pino.t};

[@bs.module]
external make: options => Express.Middleware.t = "express-pino-logger";
