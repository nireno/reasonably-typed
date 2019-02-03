type options;

[@bs.module "pug"] external renderFile: (string, options) => string = "";
let renderFile = (path, options) => renderFile(path, options |> Obj.magic);

[@bs.module "pug"] external renderStaticFile: string => string = "renderFile";

[@bs.module "pug"]
external renderFileAsync:
  NodeUtil.asyncCallback2(string, options, 'err, string) =
  "renderFile";

let renderFileAsync: NodeUtil.asyncPromise2(string, options, string) =
  NodeUtil.promisify2(renderFileAsync);

let renderFileAsync = (path, options) =>
  renderFileAsync(. path, options |> Obj.magic);
