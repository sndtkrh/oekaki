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
  fills: FillSelector.t
};

let splitList = (pred : 'a => bool, l: list('a)) => {
  let rec f = (left, right) => {
    switch (right) {
    | [] => (List.rev(left), [])
    | [h, ...rest] => {
        if (pred(h)) {
          (List.rev(left), rest)
        } else {
          f([h, ...left], rest)
        }
      }
    }
  };
  f([], l)
};

let findCoordinate = (cs, name) => {
  let f = (c) => {c.Coordinate.coordinate_id === name};
  if (List.exists (f , cs)) {
    Some(List.find(f, cs))
  } else {
    None
  }
};

let findEdge = (es, name : string) => {
  let f = (e) => {e.Edge.edge_id === name};
  if (List.exists (f, es)) {
    Some(List.find(f, es))
  } else {
    None
  }
};

let findEdges = (es, names : list(string)) => {
  let f = (name, l') => {
    switch(l') {
    | Some(l) => {
        switch(findEdge(es, name)) {
        | Some(e) => Some([e, ...l])
        | None => None
        }
      }
    | None => None
    }
  };
  List.fold_right(f, names, Some([]));
};

let addCoordinate = (setState, name, x, y) => {
  setState( (s) => {
  let cs = s.coordinates;
  let np = s.namePool;
  let nameExists =
    switch(NamePool.find_opt(name, np)){
    | Some(_) => true
    | None => false
    };
  if (name === "" ) {
    s
  } else {
    let elm : Coordinate.t = {coordinate_id: name, x: x, y: y};
    if (nameExists) {
      let (left, right) = {
        let p = (c) => {c.Coordinate.coordinate_id === name};
        splitList(p, cs)
      };
      {...s, coordinates: (left @ [elm] @ right)}
    } else {
      let newnp = NamePool.add(name, np);
      {...s, namePool: newnp, coordinates: [elm, ...cs]}
    }
  }});
};

let addEdge = (setState, name, sname, tname, curve) => {
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
      if (name === "") {
        s
      } else{
        let elm : Edge.t = {edge_id: name, scoord: sname, tcoord: tname, curve};
        if (nameExists) {
          let (left, right) = {
            let p = (e) => {e.Edge.edge_id === name};
            splitList(p, es)
          };
          {...s, edges: left @ [elm] @ right}
        } else {
          let newnp = NamePool.add(name, np);
          {...s, namePool: newnp, edges: [elm, ...es]}
        }
      }
    }
  | _ => s
  }})
};

let addFill = (setState, name, color, enames) => {
  setState( (s) => {
  let np = s.namePool;
  let es = s.edges;
  let fs = s.fills;
  switch(findEdges(es, enames)) {
  | Some(_edges) => {
      let nameExists =
        switch(NamePool.find_opt(name, np)){
        | Some(_) => true
        | None => false };
      if (name === "") {
        s
      }else {
        let elm : Fill.t = {fill_id: name, color, edges: enames};
        if (nameExists) {
          let (left, right) = {
            let p = (e) => {e.Fill.fill_id === name};
            splitList(p, fs)
          };
          {...s, fills: left @ [elm] @ right}
        } else {
          let newnp = NamePool.add(name, np);
          {...s, namePool: newnp, fills: [elm, ...fs]}
        }
      }
    }
  | _ => s
  }
  })
};

let usedCoordinate = (name, state) => {
  let a = (b, edge) => {
    if (b) { true } else {
      (edge.Edge.scoord === name || edge.Edge.tcoord === name)
    }
  };
  List.fold_left(a, false, state.edges)
};

let usedEdge = (name, state) => {
  let a = (b, fill) => {
    if (b) { true } else {
      let a' = (b', ename) => {
        switch (findEdge(state.edges, ename)) {
        | None => false
        | Some(edge) =>
          if (b') { true } else {
            edge.Edge.edge_id === name
          }
        }
      };
      List.fold_left(a' , false, fill.Fill.edges);
    }
  };
  List.fold_left(a, false, state.fills)
}
let remove = (setState, name) => {
  setState( (s) => {
  if (usedCoordinate(name, s) || usedEdge(name, s)) {
    s
  } else {
    let (cl, cr) = splitList((c) => {c.Coordinate.coordinate_id === name}, s.coordinates);
    let (el, er) = splitList((c) => {c.Edge.edge_id === name}, s.edges);
    let (fl, fr) = splitList((c) => {c.Fill.fill_id === name}, s.fills);
    {...s, coordinates: cl @ cr, edges: el @ er, fills: fl @ fr}
  }})
};
