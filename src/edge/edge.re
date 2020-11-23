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

let dirToString = (d) => {
  switch (d) {
  | Above => "above"
  | Below => "below"
  | Left  => "left"
  | Right => "right"
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
    sdir: option(direction),
    tdir: option(direction)
  };
  let select = (selectedDir, dirOnChange) => {
    let f = (lineOrBezier) => {
      switch(lineOrBezier) {
      | None => "-"
      | Some(d) => dirToString(d)
      }
    };
    let opt = (c) => {
      <option value={f(c)}> {React.string(f(c))} </option>
    };
    <select value={f(selectedDir)} onChange={dirOnChange}>
        {opt(None)}
      {opt(Some(Above))}
      {opt(Some(Below))}
      {opt(Some(Left))}
      {opt(Some(Right))}
    </select>
  };
  let initState = {name: "", sname: "", tname: "", sdir: None, tdir: None};
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
      let sdirOnChange = (e) => {
        ReactEvent.Form.preventDefault(e);
        let dirStr = ReactEvent.Form.target(e)##value;
        setState( (s) => {
          let (sdir, tdir) = {
            switch (stringToDirection(dirStr)) {
            | Some(d) => (Some(d), s.tdir)
            | None => (None, None)
            }
          };
          {...s, sdir, tdir}
        });
      };
      let tdirOnChange = (e) => {
        ReactEvent.Form.preventDefault(e);
        let dirStr = ReactEvent.Form.target(e)##value;
        setState( (s) => {
          let (sdir, tdir) = {
            switch (stringToDirection(dirStr)) {
            | Some(d) => (s.sdir, Some(d))
            | None => (None, None)
            }
          };
          {...s, sdir, tdir}
        });
      };
      let handleSubmit = (e) => {
        ReactEvent.Form.preventDefault(e);
        let curve =
          switch (state.sdir, state.tdir) {
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
        {select(state.sdir, sdirOnChange)}
      {React.string(", end=")}
      <input className="input-name" type_="text" name="tname"
          value={state.tname} onChange={handleTname}/>
        {select(state.tdir, tdirOnChange)}
      <input type_="submit" value="add/modify" onClick={_ => ()}/>
      </form>
      </div>
    };
};
