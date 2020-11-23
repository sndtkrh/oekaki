
type fill = {
  fill_id: string,
  color: Color.t,
  edges: list(string)
};

module Fill : (Cell_selector.Cell with type t = fill) = {
  type t = fill;
  let name = "fill"
    let render = (fill) => {
      let s =
        Printf.sprintf(
        "(%s) %s",
        fill.fill_id,
        String.concat(" ", fill.edges)
      );
      <div key={fill.fill_id}>
          {React.string(s)}
      </div>
    };
};

include Fill;

module Add = {
  type t = {
    name: string,
    color: Color.t,
    enames: list(string),
    ename: string
  };
  let initState = {name: "", color: Black, enames: [], ename: ""};
  [@react.component]
    let make = (~updater) => {
      let (state, setState) = {
        React.useState( () => initState)
      };

      let enamesStr = String.concat(" ", List.rev(state.enames));

      let colorOnChange = (e) => {
        ReactEvent.Form.preventDefault(e);
        let c = Color.fromString(ReactEvent.Form.target(e)##value);
        setState( (s) => {
        switch (c) {
        | Some(color) => { {...s, color} }
        | None => s
        }})
      };

      let handleName = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, name: a} );
      };

      let handleEname = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, ename: a} );
      };
        
      let handleAppend = (e) => {
        ReactEvent.Mouse.preventDefault(e);
        setState( _ => {
          {...state, enames: [state.ename, ...state.enames], ename: ""}
        });
      };

      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
        updater(state.name, state.color, List.rev(state.enames));
        setState( _ => initState );
      };

      <div>
        <form onSubmit={handleSubmit}>
            {React.string("Fill: ")}
        {React.string("name=")}
      <input className="input-name" type_="text" name="name"
          value={state.name} onChange={handleName}/>
        {Color.select(state.color, colorOnChange)}
      <input className="input-name" type_="text" name="ename"
          value={state.ename} onChange={handleEname}/>
      <button onClick={handleAppend}> {React.string("append")} </button>
      <input type_="submit" value="add/modify" onClick={_ => ()}/>
        {React.string(enamesStr)}
    </form>
  </div>
    };
};
