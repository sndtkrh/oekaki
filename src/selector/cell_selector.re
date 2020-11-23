module type Cell = {
  type t;
  let name : string;
  let render : t => React.element;
};

module MakeCellSelector(C : Cell) = {
  type t = list(C.t);
  let empty : t = [];

  [@react.component]
    let make = (~cs) => {
      <div className="selector">
        <div className="selector-header">{React.string(C.name)}</div>
        <div className={"overflow-y" ++ " " ++ C.name}>
          <>{
            ReasonReact.array(
            Array.of_list(
              List.map ( (c) => {C.render(c)}, cs)
            ))}</>
        </div>
      </div>
    };
};
