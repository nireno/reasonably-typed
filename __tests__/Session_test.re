open Jest;
open Express;

let listen = app =>
  Js.Promise.make((~resolve, ~reject) => {
    let onListen = e =>
      switch (e) {
      | exception e => reject(. e)
      | _ => resolve(. "ok")
      };
    App.listen(app, ~port=3000, ~onListen, ()) |> ignore;
  });

let agent = Superagent.agent(Superagent.request);

let () =
  describe("Session", () => {
    beforeAllPromise(() => {
      let app = express();

      App.use(
        app,
        ExpressSession.(
          make(
            options(
              ~secret="secret",
              ~resave=false,
              ~saveUninitialized=false,
              ~cookie=cookieOptions(~secure=false, ()),
            ),
          )
        ),
      );
      App.use(app, Flash.make());

      App.get(app, ~path="/test-flash") @@
      Middleware.from((_next, req, res) => {
        Flash.flash(req, "info", "foo-flash");
        Response.redirect("/get-flash", res);
      });

      App.get(app, ~path="/get-flash") @@
      Middleware.from((_next, _req, res) => {
        let messages = Flash.messages(res);
        Response.sendString(Flash.messageGet(messages[0]), res);
      });

      App.get(app, ~path="/test-state") @@
      Middleware.from((_next, req, res) => {
        ExpressSession.set(req, "foo", "foo-state");
        Response.redirect("/get-state", res);
      });

      App.get(app, ~path="/get-state") @@
      Middleware.from((_next, req, res) => {
        let foo = ExpressSession.get(req, "foo");
        Response.sendString(foo, res);
      });

      listen(app);
    });

    testPromise("keeps flash messages available after redirect", () =>
      Superagent.get(agent, "http://localhost:3000/test-flash")
      |> Js.Promise.then_(response =>
           switch (Superagent.textGet(response)) {
           | "foo-flash" => Js.Promise.resolve(pass)
           | text => failwith("Expected  but got" ++ text)
           }
         )
    );
    testPromise("keeps session state available after redirect", () =>
      Superagent.get(agent, "http://localhost:3000/test-state")
      |> Js.Promise.then_(response =>
           switch (Superagent.textGet(response)) {
           | "foo-state" => Js.Promise.resolve(pass)
           | text => failwith({j|Expected bar but got $text|j})
           }
         )
    );
  });
