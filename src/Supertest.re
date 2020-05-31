type t;
module Response = {
  type t;
  [@bs.get] external text: t => string = "text";
};

[@bs.module] external make: Express.App.t => t = "supertest";
[@bs.module "supertest"] external agent: Express.App.t => t = "agent";

[@bs.send] external get: (t, string) => Js.Promise.t(Response.t) = "get";
