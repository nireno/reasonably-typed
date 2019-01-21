type t;
[@bs.module] external make: 'options => t = "pino";

[@bs.send] external info: (t, 'a) => unit = "";
[@bs.send] external info2: (t, 'a, 'b) => unit = "info";
[@bs.send] external debug: (t, 'a) => unit = "";
[@bs.send] external debug2: (t, 'a, 'b) => unit = "debug";
[@bs.send] external warn: (t, 'a) => unit = "warn";
[@bs.send] external warn2: (t, 'a, 'b) => unit = "warn";
[@bs.send] external error: (t, 'a) => unit = "error";
[@bs.send] external error2: (t, 'a, 'b) => unit = "error";
[@bs.send] external fatal: (t, 'a) => unit = "fatal";
[@bs.send] external fatal2: (t, 'a, 'b) => unit = "fatal";
