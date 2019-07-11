open Express;
let app = express();
let (pinoInstance, pino) = Pino.make();

pino.error("where are you errror");

App.use(app, Helmet.make());
App.use(app, ExpressPinoLogger.(options(~logger=pinoInstance) |> make));

App.get(app, ~path="/") @@
Middleware.from((_next, _req, res) =>
  Response.sendString("Hello world", res)
);

let onListen = (port, e) =>
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    pino.error2("Error onListen: ", e);
    Node.Process.exit(1);
  | _ => pino.info2("Listening at http://127.0.0.1:", port)
  };

let httpPort = 3333;
Express.App.listen(app, ~port=httpPort, ~onListen=onListen(httpPort), ());
