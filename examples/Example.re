open Express;
let app = express();
let pino = Pino.make();

App.use(app, Helmet.make());

App.get(app, ~path="/") @@
Middleware.from((_next, _req, res) =>
  Response.sendString("Hello world", res)
);

let onListen = (port, e) =>
  switch (e) {
  | exception (Js.Exn.Error(e)) => {
    Pino.error2(pino, "Error onListen: ", e);
    Node.Process.exit(1);
  }
  | _ => Pino.info2(pino, "Listening at http://127.0.0.1:", port)
  };

let httpPort = 3333;
Express.App.listen(app, ~port=httpPort, ~onListen=onListen(httpPort), ());
