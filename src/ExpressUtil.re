open Express;

type options;

module Settings = {
  [@bs.send] external set: (App.t, string, 'a) => unit = "set";
  [@bs.send] external get: (App.t, string) => 'a = "get";
};

[@bs.send]
external render: (Response.t, string, options) => Express.complete = "render";
let render = (app, path, options) => render(app, path, options |> Obj.magic);

[@bs.send]
external renderStatic: (Response.t, string) => Express.complete = "render";
