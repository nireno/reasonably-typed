[@bs.deriving abstract]
type item = {
  [@bs.as "type"]
  type_: string,
  message: string,
};

[@bs.module] external make: unit => Express.Middleware.t = "flash";

[@bs.send] external flash: (Express.Request.t, string, string) => unit = "flash";

[@bs.get] [@bs.scope "locals"]
external messages: Express.Response.t => array(item) = "flash";
