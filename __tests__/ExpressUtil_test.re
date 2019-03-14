open Jest;
open Express;

let () =
  describe("ExpressUtil", () => {
    test("get/set value", () => {
      let app = express();
      ExpressUtil.Settings.set(app, "view engine", "pug");
      Expect.(
        expect(ExpressUtil.Settings.get(app, "view engine")) |> toBe("pug")
      );
    });

    testPromise("render static", () => {
      let app = express();
      ExpressUtil.Settings.set(app, "views", "./__tests__/views");
      Express.App.get(app, ~path="/static") @@
      Middleware.from((_next, _req, res) =>
        ExpressUtil.renderStatic(res, "static.pug")
      );
      let testapp = Supertest.make(app);
      Supertest.get(testapp, "/static")
      |> Js.Promise.then_(res => {
           let testResult =
             Expect.(
               expect(Supertest.Response.text(res))
               |> toBe("<p>A static page</p>")
             );
           Js.Promise.resolve(testResult);
         });
    });

    testPromise("render locals", () => {
      let app = express();
      ExpressUtil.Settings.set(app, "views", "./__tests__/views");

      Express.App.get(app, ~path="/locals") @@
      Middleware.from((_next, _req, res) =>
        ExpressUtil.render(res, "locals.pug", {"name": "world"})
      );

      let testapp = Supertest.make(app);
      Supertest.get(testapp, "/locals")
      |> Js.Promise.then_(res => {
           let testResult =
             Expect.(
               expect(Supertest.Response.text(res))
               |> toBe("<p>Hello world</p>")
             );
           Js.Promise.resolve(testResult);
         });
    });
  });
