open Utils;
open AppState;
module R = React;
module RR = ReasonReact;

requireCSS("./app.css");

[@react.component]
  let make = () => {
    let (state, setState) = {
      R.useState( () => {
      mode: Neutral,
      scale: 50.,
      svgy: 300.,
      svgx: 500.,
      coordinates: CoordinateSelector.empty,
      paths: EdgeSelector.empty
      } );
    };

    <div id="app">
      <h1>{React.string("Oekaki")}</h1>
      <Previewer state={state}/>

      <div id="selectors">
        <CoordinateSelector cs={state.coordinates}/>
        <EdgeSelector cs={state.paths}/>
      </div>

      <div id="message">{React.string("message: ")}</div>
      <Coordinate.Add updater={AppState.addCoordinate(setState)}/>
      <Edge.Add updater={AppState.addEdge(setState)}/>
    </div>
  };
