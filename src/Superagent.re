[@bs.deriving abstract]
type response = {
  body: Js.Json.t,
  text: string,
};

type t = (string, string) => Js.Promise.t(response);

[@bs.module] external request: t = "superagent";

[@bs.send] external agent: t => t = "";

[@bs.send] external get: (t, string) => Js.Promise.t(response) = "";
