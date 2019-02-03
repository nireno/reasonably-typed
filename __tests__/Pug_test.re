open Jest;

let () =
  describe("Pug", () => {
    test("Render file sync", () =>
      Expect.(
        expect(Pug.renderStaticFile("__tests__/views/static.pug"))
        |> toBe("<p>A static page</p>")
      )
    );
    testPromise("Render file async", () =>
      Pug.renderFileAsync("__tests__/views/locals.pug", {"name": "world"})
      |> Js.Promise.then_(result => {
           let testResult =
             Expect.(expect(result) |> toBe("<p>Hello world</p>"));
           Js.Promise.resolve(testResult);
         })
    );
  });
