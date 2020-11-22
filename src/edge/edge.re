type direction =
  | Above
  | Below
  | Left
  | Right

type curve =
  | Line
  | Bezier(direction, direction)

let stringToDirection = (s) => {
  switch (s) {
  | "a"     => Some(Above)
  | "above" => Some(Above)
  | "b"     => Some(Below)
  | "below" => Some(Below)
  | "l"     => Some(Left)
  | "left"  => Some(Left)
  | "r"     => Some(Right)
  | "right" => Some(Right)
  | _       => None
  }
};

type e = {
  edge_id: string,
  scoord: string,
  tcoord: string,
  curve: curve
};

module Edge : (Cell_selector.Cell with type t = e) = {
  type t = e;
  let name = "edge";
  let render = (e) => {
    let s = Printf.sprintf(
      "(%s) start=(%s) end=(%s) %s",
      e.edge_id,
      e.scoord,
      e.tcoord,
      switch (e.curve) {
      | Line => "line"
      | Bezier(sd, td) => {
          let f = (d) => {
            switch (d) {
            | Above => "above"
            | Below => "below"
            | Left  => "left"
            | Right => "right"
            }
          }
          Printf.sprintf("bezier(%s, %s)", f(sd), f(td));
        }
      }
      );
    <div key={e.edge_id}>
        {React.string(s)}
  </div>
  };
};

include Edge;

module Add = {
  type t = {
    name: string,
    sname: string,
    tname: string,
    sdir: string,
    tdir: string
  };
  let initState = {name: "", sname: "", tname: "", sdir: "", tdir: ""};
  [@react.component]
    let make = (~updater) => {
      let (state, setState) = {
        React.useState( () => { initState })
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
      let handleSdir = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, sdir: a} );
      };
      let handleTdir = (e) => {
        let a = ReactEvent.Form.target(e)##value;
        setState( (st) => {...st, tdir: a} );
      };
      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
        let curve =
          switch (stringToDirection(state.sdir), stringToDirection(state.tdir)) {
          | (Some(sd), Some(td)) => Bezier(sd, td)
          | _ => Line
          };
        updater(state.name, state.sname, state.tname, curve);
        setState( _ => initState );
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
      <input className="input-name" type_="text" name="sdir"
          value={state.sdir} onChange={handleSdir}/>
      {React.string(", end=")}
      <input className="input-name" type_="text" name="tname"
          value={state.tname} onChange={handleTname}/>
      <input className="input-name" type_="text" name="tdir"
          value={state.tdir} onChange={handleTdir}/>
      <input type_="submit" value="add" onClick={_ => ()}/>
      </form>
      </div>
    };
};
