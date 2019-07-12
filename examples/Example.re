open Express;
let app = express();

let logger = Pino.make();
let childLogger = logger.makeChild({"ns": "Example"});

App.use(app, Helmet.make());
App.use(
  app,
  ExpressPinoLogger.(options(~pino=logger.Pino.instance) |> make),
);

App.get(app, ~path="/") @@
Middleware.from((_next, _req, res) =>
  Response.sendString("Hello world", res)
);

let onListen = (port, e) =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    childLogger.error2("Error onListen: ", e);
    Node.Process.exit(1);
  | _ => childLogger.info2("Listening at http://127.0.0.1:", port)
  };

let httpPort = 3333;
Express.App.listen(app, ~port=httpPort, ~onListen=onListen(httpPort), ());
