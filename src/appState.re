type mode = Neutral | AddCoordinate;

type t = {
  mode: mode,
  scale: float,
  svgx: float,
  svgy: float,
  coordinates: CoordinateSelector.t,
  paths: EdgeSelector.t,
};


let findCoordinate = (cs, name) => {
  let f = (c) => {c.Coordinate.coordinate_id === name};
  if (List.exists (f , cs)) {
    Some(List.find(f, cs))
  } else {
    None
  }
};

let addCoordinate = (setState, name, x, y) => {
  let elm : Coordinate.t = {coordinate_id: name, x: x, y: y};
  setState( (s) => {
    let cs = s.coordinates;
    if (name === ""
        || switch(findCoordinate(cs, name)){ | None => true | Some(_) => false }) {
      s
    } else {
      {...s, coordinates: [elm, ...cs]}
    }
  });
};

let addEdge = (setState, name, sname, tname) => {
  setState( (s) => {
  let cs = s.coordinates;
  switch( findCoordinate(cs, sname), findCoordinate(cs, tname) ){
  | (Some(_p), Some(_q)) =>
    s
  | _ => s
  }})
};
