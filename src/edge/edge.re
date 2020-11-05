type e = {edge_id: string, scoord: string, tcoord: string};
module Edge : (Cell_selector.Cell with type t = e) = {
  type t = e;
  let name = "edge";
  let render = (c) => {
    <div>{React.string(c.edge_id)}</div>
  };
};

include Edge;

module Add = {
  type t = {name: string, sname: string, tname: string};
  [@react.component]
    let make = (~updater) => {
      let (state, setState) = {
        React.useState( () => { {name: "", sname: "", tname: ""} })
      };
      let handleName = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, name: a} );
      };
      let handleSname = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, sname: a} );
      };
      let handleTname = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, tname: a} );
      };
      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
        updater(state.name, state.sname, state.tname);
        setState( _ => {name: "", sname: "", tname: ""} );
      };
      
      <div>
          {React.string("Add edge: ")}
      <form onSubmit={handleSubmit}>
          {React.string("name=")}
      <input className="input-name" type_="text" name="name"
          value={state.name} onChange={handleName}/>
        {React.string(", start=")}
      <input className="input-name" type_="text" name="sname"
          value={state.sname} onChange={handleSname}/>
        {React.string(", end=")}
      <input className="input-name" type_="text" name="tname"
          value={state.tname} onChange={handleTname}/>
        <input type_="submit" value="add" onClick={_ => ()}/>
      </form>
      </div>
    };
};
