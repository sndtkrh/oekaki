type mode = Neutral | AddCoordinate;

module NamePool = Set.Make(String);

type t = {
  mode: mode,
  scale: float,
  svgx: float,
  svgy: float,
  namePool: NamePool.t,
  coordinates: CoordinateSelector.t,
  edges: EdgeSelector.t,
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
  setState( (s) => {
    let cs = s.coordinates;
    let np = s.namePool;
    let nameExists =
      switch(NamePool.find_opt(name, np)){
      | Some(_) => true
      | None => false };
    if (name === "" || nameExists) {
      s
    } else {
      let newnp = NamePool.add(name, np);
      let elm : Coordinate.t = {coordinate_id: name, x: x, y: y};
      {...s, namePool: newnp, coordinates: [elm, ...cs]}
    }
  });
};

let addEdge = (setState, name, sname, tname) => {
  setState( (s) => {
  let np = s.namePool;
  let es = s.edges;
  let cs = s.coordinates;
  switch( findCoordinate(cs, sname), findCoordinate(cs, tname) ){
  | (Some(_p), Some(_q)) => {
      let nameExists =
        switch(NamePool.find_opt(name, np)){
        | Some(_) => true
        | None => false };
      if (name === "" || nameExists) {
        s
      } else {
        let newnp = NamePool.add(name, np);
        let elm : Edge.t = {edge_id: name, scoord: sname, tcoord: tname};
        {...s, namePool: newnp, edges: [elm, ...es]}
      }
    }
  | _ => s
  }})
};
