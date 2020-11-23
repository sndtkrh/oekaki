type t = {name: string};

let initState = {name: ""};

[@react.component]
  let make = (~updater) => {
    let (state, setState) = {
      React.useState( () => initState)
    };
    
    let handleName = (e) => {
      let a = ReactEvent.Form.target(e)##value;
      setState( (_) => {name: a} );
    };
    
    let handleSubmit = (e) => {
      ReactEvent.Form.preventDefault(e);
      updater(state.name);
    };

    <div>
        {React.string("Delete")}
      <form onSubmit={handleSubmit}>
        <input className="input-name" type_="text" name="name"
            value={state.name} onChange={handleName}/>
      <input type_="submit" value="delete" onClick={_ => ()}/>
      </form>
    </div>
  }
