let component = ReasonReact.statelessComponent("LabelContent");

let checkBoxesStyle =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~marginBottom="1em",
    ~display="flex",
    ~justifyContent="flex-start",
    (),
  );

let checkBoxLabelStyle = ReactDOMRe.Style.make(~margin="0", ());

let checkBoxesLabelStyle = ReactDOMRe.Style.make(~marginRight="4em", ());

let make =
    (~labelContentList: list((string, ReasonReact.reactElement)), _children) => {
  ...component,
  render: _self =>
    <div style=checkBoxesStyle>
      <div style=checkBoxesLabelStyle>
        {
          labelContentList
          |> Belt.List.map(_, ((name, _)) =>
               <div key={name ++ "_content"}>
                 <label style=checkBoxLabelStyle htmlFor=name>
                   {ReasonReact.string(name)}
                 </label>
               </div>
             )
          |> Utils.ReasonReact.listToReactArray
        }
      </div>
      <div>
        {
          labelContentList
          |> Belt.List.map(_, ((id, content)) =>
               <div key={id ++ "_content"}> content </div>
             )
          |> Utils.ReasonReact.listToReactArray
        }
      </div>
    </div>,
};