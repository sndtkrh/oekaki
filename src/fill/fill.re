type edgeDir =
  | Forward
  | Reverse

let edgeDirToString = (edir) => {
  switch (edir) {
  | Forward => "forward"
  | Reverse => "reverse"
  }
};
let stringToEdgeDir = (s) => {
  if (s === "reverse") { Reverse } else { Forward }
};


type fill = {
  fill_id: string,
  color: Color.t,
  edges: list((string, edgeDir))
};

let rotateEdge = (edir, edge : Edge.t) => {
  switch (edir) {
  | Forward => edge
  | _ => {
      switch (edge.curve) {
      | Edge.Line =>
        {...edge, scoord: edge.tcoord, tcoord: edge.scoord}
      | Edge.Bezier(sdir, tdir) =>
        {...edge, scoord: edge.tcoord, tcoord: edge.scoord, curve: Bezier(tdir, sdir) }
      }
    }
  }
};

let edgesToString = (edges: list((string, edgeDir))) => {
  String.concat(
  " ",
  List.map( ((s,ed)) => {
    switch (ed) {
    | Forward => s
    | Reverse => "rev(" ++ s ++ ")"
    }
  }, edges))
};

module Fill : (Cell_selector.Cell with type t = fill) = {
  type t = fill;
  let name = "fill";
  let render = (fill) => {
    let s =
      Printf.sprintf(
      "(%s) %s",
      fill.fill_id,
      edgesToString(fill.edges)
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
    enames: list((string, edgeDir)),
    edir: edgeDir,
    ename: string
  };
  let initState = {name: "", color: Black, enames: [], edir: Forward, ename: ""};
  [@react.component]
    let make = (~updater) => {
      let (state, setState) = {
        React.useState( () => initState)
      };

      let enamesStr = edgesToString(List.rev(state.enames));

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

      let edirOnChange = (e) => {
        ReactEvent.Form.preventDefault(e);
        let edStr = ReactEvent.Form.target(e)##value;
        setState( (s) => {
          {...s, edir: stringToEdgeDir(edStr)}
        })
      };
        
      let handleAppend = (e) => {
        ReactEvent.Mouse.preventDefault(e);
        setState( _ => {
          {...state, enames: [(state.ename, state.edir), ...state.enames], ename: ""}
        });
      };

      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
        updater(state.name, state.color, List.rev(state.enames));
        setState( _ => initState );
      };

      <div className={"add" ++ " " ++ Fill.name}>
        <form onSubmit={handleSubmit}>
            {React.string("Fill: ")}
        {React.string("name=")}
      <input className="input-name" type_="text" name="name"
          value={state.name} onChange={handleName}/>
        {Color.select(state.color, colorOnChange)}
      <input className="input-name" type_="text" name="ename"
          value={state.ename} onChange={handleEname}/>
      <select value={edgeDirToString(state.edir)} onChange={edirOnChange}>
        <option value={edgeDirToString(Forward)}> {React.string(edgeDirToString(Forward))} </option>
        <option value={edgeDirToString(Reverse)}> {React.string(edgeDirToString(Reverse))} </option>
      </select>
      <button onClick={handleAppend}> {React.string("append")} </button>
      <input type_="submit" value="add/modify" onClick={_ => ()}/>
        {React.string(enamesStr)}
    </form>
  </div>
    };
};
