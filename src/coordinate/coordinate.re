type c = {
  coordinate_id: string,
  x: float,
  y: float
};

module Coordinate : (Cell_selector.Cell with type t = c) = {
  type t = c;
  let name = "coordinate";

  let render = (c) => {
    let s =
      { "(" ++ c.coordinate_id ++ ")"
        ++ " x=" ++ Belt.Float.toString(c.x)
        ++ " y=" ++ Belt.Float.toString(c.y) };
    <div key={c.coordinate_id}>
      {React.string(s)}
    </div>
  };
};

let testa = {coordinate_id: "a", x: 1., y:2.};
let testb = {coordinate_id: "b", x: 3., y:4.};

include Coordinate;

module Add = {
  type t = {name: string, x: string, y: string};
  [@react.component]
    let make = (~updater) => {
      let (state, setState) = {
        React.useState( () => {
        {name: "", x: "0", y: "0"}
        })
      };

      let handleName = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, name: a} );
      };
      let handleX = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, x: a} );
      };
      let handleY = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, y: a} );
      };

      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
        switch( (Belt.Float.fromString(state.x), Belt.Float.fromString(state.y)) ){
        | (Some(x), Some(y)) => {
            updater(state.name, x, y);
            setState( _ => {name: "", x: "", y: ""} );
          }
        | _ => ()
        };
      };

      <div className={"add" ++ " " ++ Coordinate.name}>
        <form onSubmit={handleSubmit}>
            {React.string("Coordinate: ")}
        {React.string("name=")}
      <input className="input-name" type_="text" name="name" value={state.name} onChange={handleName} />
        {React.string(", x=")}
      <input className="input-num" type_="text" name="x" value={state.x} onChange={handleX}/>
        {React.string(", y=")}
      <input className="input-num" type_="text" name="y" value={state.y} onChange={handleY}/>
      <input type_="submit" value="add/modify" onClick={_ => ()}/>
    </form>
  </div>
    }
};
