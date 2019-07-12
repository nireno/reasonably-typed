open Express;
let app = express();

let (pinoInstance, log) = Pino.make();
let (_instance, childLog) = log.makeChild({"ns": "Example"});

App.use(app, Helmet.make());
App.use(app, ExpressPinoLogger.(options(~logger=pinoInstance) |> make));

App.get(app, ~path="/") @@
Middleware.from((_next, _req, res) =>
  Response.sendString("Hello world", res)
);

let onListen = (port, e) =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    childLog.error2("Error onListen: ", e);
    Node.Process.exit(1);
  | _ => childLog.info2("Listening at http://127.0.0.1:", port)
  };

let httpPort = 3333;
Express.App.listen(app, ~port=httpPort, ~onListen=onListen(httpPort), ());
