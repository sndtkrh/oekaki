module Path : Cell_selector.Cell {
  type t = {path_id: string};
  let name = "path";
  let render = (c) => {
    <div>{React.string(c.path_id)}</div>
  };
};
include Path
