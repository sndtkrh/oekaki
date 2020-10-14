type mode = Neutral | AddCoordinate;

type t = {
  mode: mode,
  scale: float,
  svgx: float,
  svgy: float,
  coordinates: CoordinateSelector.t,
  paths: PathSelector.t,
};


let addCoordinate = (setState, name, x, y) => {
  let elm : Coordinate.t = {coordinate_id: name, x: x, y: y};
  setState( (s) => {
  {...s, coordinates: [elm, ...s.coordinates]}
  });
};
