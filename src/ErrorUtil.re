exception PromiseFailure(Js.Promise.error);

let rec unwrap: Js.Promise.error => 'a =
  err => {
    let x = err |> Obj.magic |> snd;
    Js.Types.(
      switch (classify(x)) {
      | JSUndefined => err |> Obj.magic
      | _ => unwrap(x)
      }
    );
  };

[@bs.get] external getCode: 'a => string = "code";
