[@bs.val] external dbHost: string = "process.env.db_host_env";
[@bs.val] external dbName: string = "process.env.db_name_env";
[@bs.val] external dbPort: string = "process.env.db_port_env";
[@bs.val] external dbUser: string = "process.env.db_user_env";
[@bs.val] external dbPass: string = "process.env.db_pass_env";
open Jest;
open Express;

let pool =
  Pg.makePool(
    ~database=dbName,
    ~host=dbHost,
    ~user=dbUser,
    ~password=dbPass,
    ~port=int_of_string(dbPort),
    (),
  );

let pgstore =
  PgSessionStore.make(
    ExpressSession.session,
    PgSessionStore.options(~pool, ()),
  );

let app = express();

App.use(
  app,
  ExpressSession.(
    session(
      options(
        ~store=pgstore,
        ~secret="secret",
        ~resave=false,
        ~saveUninitialized=false,
        ~cookie=cookieOptions(~secure=false, ~maxAge=3000, ()),
        (),
      ),
    )
  ),
);
App.use(app, Flash.make());

App.get(app, ~path="/flash-test") @@
PromiseMiddleware.from((_next, req, res) => {
  Flash.flash(req, "info", "flash-foo");
  ExpressSession.save(. req)
  |> Js.Promise.then_(_ =>
       Js.Promise.resolve(Response.redirect("/flash-get", res))
     );
});

App.get(app, ~path="/flash-get") @@
Middleware.from((_next, _req, res) =>
  switch (Flash.messages(res)) {
  | [|hd|] => Response.sendString(Flash.messageGet(hd), res)
  | _ => Response.sendString("No flash messages found", res)
  }
);

App.get(app, ~path="/state-test") @@
PromiseMiddleware.from((_next, req, res) => {
  ExpressSession.set(req, "foo", "state-foo");
  ExpressSession.save(. req)
  |> Js.Promise.then_(_ =>
       Js.Promise.resolve(Response.redirect("/state-get", res))
     );
});

App.get(app, ~path="/state-get") @@
Middleware.from((_next, req, res) => {
  let foo = ExpressSession.get(req, "foo");
  Response.sendString(foo, res);
});

let agent = Supertest.agent(app);

let () =
  describe("Session", () => {
    testPromise("keeps flash messages available after redirect", () =>
      Supertest.get(agent, "/flash-test")
      |> Js.Promise.then_(_ => Supertest.get(agent, "/flash-get"))
      |> Js.Promise.then_(response => {
           let testResponse =
             Expect.(
               expect(Supertest.Response.text(response))
               |> toBe("flash-foo")
             );
           Js.Promise.resolve(testResponse);
         })
    );
    testPromise("keeps session state available after redirect", () =>
      Supertest.get(agent, "/state-test")
      |> Js.Promise.then_(_ => Supertest.get(agent, "/state-get"))
      |> Js.Promise.then_(response => {
           let testResponse =
             Expect.(
               expect(Supertest.Response.text(response))
               |> toBe("state-foo")
             );
           Js.Promise.resolve(testResponse);
         })
    );
  });
