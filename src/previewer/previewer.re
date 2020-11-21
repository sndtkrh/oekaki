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

let coordToSVG = (state, c) => {
  let (x, y) = convert(c.Coordinate.x, c.Coordinate.y, state);
  <circle key={c.Coordinate.coordinate_id}
      cx={Belt.Float.toString(x)}
    cy={Belt.Float.toString(y)}
    r={"2"}
    stroke={"black"}
    fill={"none"}/>
};

let distControlPoint = 1.0;
let controlPoint = (x, y, d) => {
  switch (d) {
  | Edge.Above => (x, y +. distControlPoint)
  | Edge.Below => (x, y -. distControlPoint)
  | Edge.Left  => (x -. distControlPoint, y)
  | Edge.Right => (x +. distControlPoint, y)
  }
};

let edgeToSVG = (state, e) => {
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
  | Edge.Line => {
      let d = Printf.sprintf("M %f %f L %f %f", sX, sY, tX, tY);
      <path key={e.Edge.edge_id} d={d} stroke={"black"} fill={"none"}/>
  };
  | Edge.Bezier(sd, td) => {
      let (scX, scY) = convertP(controlPoint(sx, sy, sd), state);
      let (tcX, tcY) = convertP(controlPoint(tx, ty, td), state);
      let d = Printf.sprintf(
        "M %f %f C %f %f, %f %f, %f %f",
        sX, sY,
        scX, scY, tcX, tcY, tX, tY);
      <path key={e.Edge.edge_id} d={d} stroke={"black"} fill={"none"}/>
    };
  };
};

[@react.component]
  let make = (~state) => {
    let coords = {
      RR.array( Array.of_list(
      List.concat([
        // coordinate
        List.map(coordToSVG(state), state.coordinates),
        // edge
        List.map(edgeToSVG(state), state.edges)
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
