open Jest;

let () =
  describe("Shortid", () =>
    test("generate", () => {
      let length = Shortid.generate() |> String.length;

      Expect.(expect(length) |> toBeGreaterThanOrEqual(7));
    })
  );
