open Jest;

let () =
  describe("Pug", () => {
    test("Render file sync", () =>
      Expect.(
        expect(Pug.renderStaticFile("__tests__/views/view.pug"))
        |> toBe("<html><body><h1>Hello </h1></body></html>")
      )
    );
    testPromise("Render file async", () =>
      Pug.renderFileAsync(.
        "__tests__/views/view.pug",
        {"name": "foo"} |> Obj.magic,
      )
      |> Js.Promise.then_(result => {
           let testResult =
             Expect.(
               expect(result)
               |> toBe("<html><body><h1>Hello foo</h1></body></html>")
             );
           Js.Promise.resolve(testResult);
         })
    );
  });
