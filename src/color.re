type t =
  | Black
  | White
  | Red
  | Green
  | Blue

let fromString = (s) => {
  switch(s) {
  | "black" => Some(Black)
  | "white" => Some(White)
  | "red"   => Some(Red)
  | "green" => Some(Green)
  | "blue"  => Some(Blue)
  | _ => None
  }
};

let toString = (c) => {
  switch (c) {
  | Black => "black"
  | White => "white"
  | Red   => "red"
  | Green => "green"
  | Blue  => "blue"
  }
};

let select = (selectedColor, colorOnChange) => {
  let opt = (c) => {
    <option key={toString(c)} value={toString(c)}>
        {React.string(toString(c))}
    </option>
  };
  <select value={toString(selectedColor)} onChange={colorOnChange}>
      {opt(Black)}
    {opt(Red)}
    {opt(Green)}
    {opt(Blue)}
    {opt(White)}
  </select>
};
