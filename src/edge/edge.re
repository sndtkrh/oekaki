module Edge : Cell_selector.Cell {
  type t = {path_id: string};
  let name = "edge";
  let render = (c) => {
    <div>{React.string(c.path_id)}</div>
  };
};
include Edge;

module Add = {
  type = {name: string, sc: string, tc: string};
  [@react.component]
    let make = (~updater) => {
      let (state, setState) = {
        React.useState( () => { {name: "", sc: "", tc: ""} })
      };
      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
      };
      
      <div>
          {React.string("Add edge: ")}
      <form onSubmit={handleSubmit}>
          {React.string("name=")}
      <input className="input-name" type_="text" name="name"/>
        {React.string(", start=")}
      <input className="input-name" type_="text" name="sname"/>
        {React.string(", end=")}
      <input className="input-name" type_="text" name="tname"/>
        <input type_="submit" value="add" onClick={_ => ()}/>
      </form>
      </div>
    };
};
