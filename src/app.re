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
      namePool: AppState.NamePool.empty,
      coordinates: CoordinateSelector.empty,
      edges: EdgeSelector.empty,
      fills: FillSelector.empty
      } );
    };

    <div id="app">
      <h1>{React.string("Oekaki")}</h1>
      <div id="output">
        <Previewer state={state}/>
        <Tikz state={state}/>
      </div>

      <div id="selectors">
        <CoordinateSelector cs={state.coordinates}/>
        <EdgeSelector cs={state.edges}/>
        <FillSelector cs={state.fills}/>
      </div>

      <div id="message">{React.string("message: ")}</div>
      <Coordinate.Add updater={AppState.addCoordinate(setState)}/>
      <Edge.Add updater={AppState.addEdge(setState)}/>
      <Fill.Add updater={AppState.addFill(setState)}/>
    </div>
  };
