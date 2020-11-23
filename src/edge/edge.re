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
  | "above" => Some(Above)
  | "below" => Some(Below)
  | "left"  => Some(Left)
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
  curve: curve,
  draw: bool
};

module Edge : (Cell_selector.Cell with type t = e) = {
  type t = e;
  let name = "edge";
  let render = (e) => {
    let s = Printf.sprintf(
      "(%s) draw=%s (%s) %s (%s)",
      e.edge_id,
      if (e.draw) {"yes"} else {"no"},
      e.scoord,
      switch (e.curve) {
      | Line => "--"
      | Bezier(sd, td) => {
          let f = (d) => {
            switch (d) {
            | Above => "above"
            | Below => "below"
            | Left  => "left"
            | Right => "right"
            }
          }
          Printf.sprintf("~(%s, %s)", f(sd), f(td));
        }
      },
      e.tcoord
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
    draw: bool,
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
  let initState = {name: "", draw:true, sname: "", tname: "", sdir: None, tdir: None};
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
      let drawOnChange = (e) => {
        ReactEvent.Form.preventDefault(e);
        let draw = ReactEvent.Form.target(e)##value;
        setState( (s) => {
          switch (draw) {
          | "yes" => {...s, draw:true}  // yes
          | _     => {...s, draw:false} // no
          }
        })
      }
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
        updater(state.name, state.draw, state.sname, state.tname, curve);
        setState( _ => initState );
      };
      
      <div className={Edge.name}>
        <form onSubmit={handleSubmit}>
            {React.string("Edge: ")}
        {React.string("name=")}
      <input className="input-name" type_="text" name="name"
          value={state.name} onChange={handleName}/>
        {React.string(" draw:")}
      <select value={if(state.draw) {"yes"} else {"no"}} onChange={drawOnChange}>
        <option value="yes">{React.string("yes")}</option>
        <option value="no" >{React.string("no")} </option>
      </select>
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
