open AppState;
module RR = ReasonReact;

let convert = (a, b, st) => {
  let x = a *. st.scale;
  let y = st.svgy -. b *. st.scale;
  (x,y)
};

let convertP = ((a,b), st) => {
  convert(a, b, st)
};

let fontSize="20"

let coordToSVG = (state, c) => {
  let (x, y) = convert(c.Coordinate.x, c.Coordinate.y, state);
  [
    <circle key={"coord_" ++ c.Coordinate.coordinate_id}
        cx={Belt.Float.toString(x)}
      cy={Belt.Float.toString(y)}
      r={"2"}
      stroke={"black"}
      fill={"none"}/>,
    <text key={"text_" ++ c.Coordinate.coordinate_id}
        textAnchor="middle"
        x={Belt.Float.toString(x)}
      y={Belt.Float.toString(y -. 8.)}
    fontSize={fontSize}>
      {React.string(c.coordinate_id)}
  </text>
  ]
};

let distControlPoint = 0.5;
let controlPoint = (x, y, d) => {
  switch (d) {
  | Edge.Above => (x, y +. distControlPoint)
  | Edge.Below => (x, y -. distControlPoint)
  | Edge.Left  => (x -. distControlPoint, y)
  | Edge.Right => (x +. distControlPoint, y)
  }
};
let edgeCurveD = (state, e) => {
  let cs = state.coordinates;
  let sname = e.Edge.scoord;
  let tname = e.Edge.tcoord;
  let (sx, sy) = switch(findCoordinate(cs, sname)){
    | Some(c) => (c.Coordinate.x, c.Coordinate.y);
    | None => (0., 0.); // connot happen
  };
  let (tx, ty) = switch(findCoordinate(cs, tname)){
    | Some(c) => (c.Coordinate.x, c.Coordinate.y);
    | None => (0., 0.); // connot happen
  };
  let (sX, sY) = convert(sx, sy, state);
  let (tX, tY) = convert(tx, ty, state);
  switch (e.Edge.curve) {
  | Edge.Line =>
    (Printf.sprintf("M %f %f", sX, sY),
     Printf.sprintf("L %f %f", tX, tY))
  | Edge.Bezier(sd, td) => {
      let (scX, scY) = convertP(controlPoint(sx, sy, sd), state);
      let (tcX, tcY) = convertP(controlPoint(tx, ty, td), state);
      (Printf.sprintf("M %f %f", sX, sY),
       Printf.sprintf("C %f %f, %f %f, %f %f", scX, scY, tcX, tcY, tX, tY));
    }
  }
};

let edgeToSVG = (state, e) => {
  let (m, c) = edgeCurveD(state, e);
  let d = m ++ c;
  [
  <path key={e.Edge.edge_id}
      id={e.Edge.edge_id} d={d}
    stroke={if (e.draw) {"black"} else {"lightseagreen"} }
    fill={"none"}/>,
  <text key={"text_" ++ e.Edge.edge_id}
      textAnchor="middle"
    fontSize={fontSize} fill="blue" >
  <textPath key={"textPath_" ++ e.Edge.edge_id}
            startOffset="50%"
    xlinkHref={"#" ++ e.Edge.edge_id}>
    {React.string(e.Edge.edge_id)}
</textPath>
    </text>
  ]
};

let fillToSVG = (state, fill : Fill.t) => {
  let d = (edges : list(Edge.t)) => {
    let rec func = (prevCoordName, es) => {
      switch (es) {
      | [] => "z"
      | [e, ...rest] => {
          let (_, c) = edgeCurveD(state, e);
          if (prevCoordName === e.Edge.scoord) {
            c ++ func(e.Edge.tcoord, rest)
          } else {
            switch(findCoordinate(state.coordinates, e.Edge.scoord)) {
            | Some(c) => {
                let (cX, cY) = convert(c.x, c.y, state);
                Printf.sprintf("L %f %f", cX, cY)
              }
            | None => "" // cannot happen
            } ++ c ++ func(e.Edge.tcoord, rest)
          }
        }
      }
    };
    switch (edges) {
    | [] => ""
    | [head, ...rest] => {
        let (m, c) = edgeCurveD(state, head);
        m ++ c ++ func(head.Edge.tcoord, rest)
      }
    }
  };
  switch (findEdges(state.edges, fill.edges)) {
  | Some(edges) => {
      <path key={fill.fill_id} d={d(edges)}
          stroke="none" fill={Color.toString(fill.color)} />
    }
  | None => { <> </> }
  }
};

[@react.component]
  let make = (~state) => {
    let coords = {
      RR.array( Array.of_list(
      List.concat([
        //fill
        List.map(fillToSVG(state), state.fills),
        // coordinate
        List.concat(List.map(coordToSVG(state), state.coordinates)),
        // edge
        List.concat(List.map(edgeToSVG(state), state.edges))
      ])
      ))
    };

    <div className="preview">
      <svg x="0" y="0"
          width={Belt.Float.toString(state.svgx)}
      height={Belt.Float.toString(state.svgy)} >
          {coords}
      </svg>
    </div>
  };
