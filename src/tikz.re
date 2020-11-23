open AppState;
let beginTikz = "\\begin{tikzpicture}";
let endTikz = "\\end{tikzpicture}";

let fromCoordinate = (c) => {
  Printf.sprintf(
  "\\coordinate (%s) at (%f, %f);",
  c.Coordinate.coordinate_id,
  c.Coordinate.x,
  c.Coordinate.y);
};

let fromDir = (dir) => {
  switch (dir) {
  | Edge.Above => "90"
  | Edge.Below => "-90"
  | Edge.Left => "180"
  | Edge.Right => "0"
  }
};
let fromDirs = (d1, d2) => {
  Printf.sprintf("to[out=%s, in=%s]", fromDir(d1), fromDir(d2));
};
let fromEdge' = (e : Edge.t) => {
  ("(" ++ e.Edge.scoord ++ ")",
   switch (e.Edge.curve) {
   | Edge.Line =>
     Printf.sprintf(" -- (%s)", e.tcoord);
   | Edge.Bezier(d1, d2) =>
     Printf.sprintf(" %s (%s)", fromDirs(d1, d2), e.tcoord);
   })
};
let fromEdge = (e : Edge.t) => {
  if (e.draw) {
    let (s, t) = fromEdge'(e);
    Printf.sprintf("\\draw %s;", s ++ t);
  } else {
    ""
  }
};

let fromFill = (state, fill : Fill.t) => {
  let rec func = (prevCoord, edges) => {
    switch(edges) {
    | [] => "-- cycle"
    | [e, ...es] => {
        if (prevCoord === e.Edge.scoord) {
          let (_, t) = fromEdge'(e);
          t ++ func(e.Edge.tcoord, es);
        } else {
          let (s, t) = fromEdge'(e);
          " -- " ++ s ++ t ++ func(e.Edge.tcoord, es);
        }
      }
    }
  };

  switch (findEdges(state.edges, fill.Fill.edges)) {
  | None => ""
  | Some([]) => ""
  | Some([head, ...rest]) => {
      let (s, t) = fromEdge'(head);
      Printf.sprintf("\\fill[%s] %s %s;",
                     Color.toString(fill.color),
                     s ++ t,
                     func(head.Edge.tcoord, rest)
      )
    }
  }
};

[@react.component]
  let make = (~state) => {
    let coordinatesTikz =
      List.map(fromCoordinate, state.coordinates);
    let fillsTikz =
      List.map(fromFill(state), state.fills);
    let edgesTikz =
      List.filter( (str) => {!(str === "")}, List.map(fromEdge, state.edges));
    let tikz = {
      String.concat("\n", [beginTikz] @ coordinatesTikz @ fillsTikz @ edgesTikz @ [endTikz])
    };
    
    <div id="">
      <textarea className="tikz" value={tikz} />
    </div>
  };
