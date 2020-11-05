open AppState;
module RR = ReasonReact;

let convert = (a, b, st) => {
  let x = a *. st.scale;
  let y = st.svgy -. b *. st.scale;
  (x,y)
};

[@react.component]
  let make = (~state) => {
    let cs = state.coordinates;
    let es = state.edges;
    let coords = {
      RR.array( Array.of_list(
      List.concat([
        List.map( (c) => {
        let (x, y) = convert(c.Coordinate.x, c.Coordinate.y, state);
        <circle key={c.Coordinate.coordinate_id}
            cx={Belt.Float.toString(x)}
          cy={Belt.Float.toString(y)}
          r={"2"}
          stroke={"black"}
          fill={"none"}/>
        }, cs),
        List.map( (e) => {
        let sname = e.Edge.scoord;
        let tname = e.Edge.tcoord;
        let (sx, sy) = switch(findCoordinate(cs, sname)){
          | Some(c) => convert(c.Coordinate.x, c.Coordinate.y, state);
          | None => (0., 0.);
        };
        let (tx, ty) = switch(findCoordinate(cs, tname)){
          | Some(c) => convert(c.Coordinate.x, c.Coordinate.y, state);
          | None => (0., 0.);
        };
        let d = Printf.sprintf("M %f,%f L %f,%f", sx, sy, tx, ty);
        <path key={e.Edge.edge_id} d={d} stroke={"black"} fill={"none"}/>
        }, es)
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
