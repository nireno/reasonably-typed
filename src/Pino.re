type t;
[@bs.module] external instantiate: 'options => t = "pino";

[@bs.send] external info: (t, 'a) => unit = "info";
[@bs.send] external info2: (t, 'a, 'b) => unit = "info";
[@bs.send] external debug: (t, 'a) => unit = "debug";
[@bs.send] external debug2: (t, 'a, 'b) => unit = "debug";
[@bs.send] external warn: (t, 'a) => unit = "warn";
[@bs.send] external warn2: (t, 'a, 'b) => unit = "warn";
[@bs.send] external error: (t, 'a) => unit = "error";
[@bs.send] external error2: (t, 'a, 'b) => unit = "error";
[@bs.send] external fatal: (t, 'a) => unit = "fatal";
[@bs.send] external fatal2: (t, 'a, 'b) => unit = "fatal";
[@bs.send] external child: (t, 'a) => t = "child";

type logger = {
  instance: t,
  info:'a . 'a => unit,
  info2: 'a 'b. ('a, 'b) => unit,
  debug:'a . 'a => unit,
  debug2: 'a 'b. ('a, 'b) => unit,
  warn:'a . 'a => unit,
  warn2: 'a 'b. ('a, 'b) => unit,
  error:'a . 'a => unit,
  error2: 'a 'b. ('a, 'b) => unit,
  fatal:'a . 'a => unit,
  fatal2: 'a 'b. ('a, 'b) => unit,
  makeChild: 'a . 'a => logger
};


/** Bake the pino instance into each send call */
let rec bake = pino => {
  instance: pino,
  info: a => info(pino, a),
  info2: (a, b) => info2(pino, a, b),
  debug: a => debug(pino, a),
  debug2: (a, b) => debug2(pino, a, b),
  warn: a => warn(pino, a),
  warn2: (a, b) => warn2(pino, a, b),
  error: a => error(pino, a),
  error2: (a, b) => error2(pino, a, b),
  fatal: a => fatal(pino, a),
  fatal2: (a, b) => fatal2(pino, a, b),
  makeChild: bindings => {
    let childPino = child(pino, bindings);
    bake(childPino);
  },
};

let make = (options) => instantiate(options) |> bake
