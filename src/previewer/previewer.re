open AppState;
module RR = ReasonReact;

[@react.component]
  let make = (~state) => {
    let cs = state.coordinates;
    let coords = {
      RR.array( Array.of_list(
        List.map(
        (c) => {
          let x = c.Coordinate.x *. state.scale;
          let y = state.svgy -. c.Coordinate.y *. state.scale;
          <circle
              cx={Belt.Float.toString(x)}
            cy={Belt.Float.toString(y)}
            r={"2"}
            stroke={"black"}
            fill={"none"}/>
        },
        cs
      )))
    };


    <div className="preview">
      <svg x="0" y="0"
          width={Belt.Float.toString(state.svgx)}
      height={Belt.Float.toString(state.svgy)} >
          {coords}
      </svg>
    </div>
  };
