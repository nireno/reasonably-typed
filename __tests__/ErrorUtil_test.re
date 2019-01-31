open Jest;
type error = {. "code": string};

exception Rejection(error);

let () =
  describe("ErrorUtil", () =>
    testPromise("unboxing", () =>
      Js.Promise.reject(Rejection({"code": "ENOENT"}))
      /* ) */
      /*** First box */
      |> Js.Promise.catch(e =>
           Js.Promise.reject(ErrorUtil.PromiseFailure(e))
         )
      /*** Second box */
      |> Js.Promise.catch(e =>
           Js.Promise.reject(ErrorUtil.PromiseFailure(e))
         )
      |> Js.Promise.catch(e =>
           Js.Promise.resolve(
             Expect.(
               expect(e |> ErrorUtil.unwrap |> ErrorUtil.getCode)
               |> toBe("ENOENT")
             ),
           )
         )
    )
  );
