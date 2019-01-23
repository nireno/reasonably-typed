open Jest;

let () =
  describe("Validator", () => {
    test("Is not an email", () =>
      Expect.expect(Validator.isEmail("@example.com")) |> Expect.toBe(false)
    );
    test("Valid email", () =>
      Expect.(expect(Validator.isEmail("me@example.com")) |> toBe(true))
    );
  });
