let component = ReasonReact.statelessComponent("Editable");

let textStyle = thereIsText =>
  ReactDOMRe.Style.make(~color=thereIsText ? "black" : "gray", ());

let make =
    (
      ~style=?,
      ~className=?,
      ~editing,
      ~onTextChange,
      ~onKeyDown=?,
      ~editingToggle,
      ~value,
      ~placeholder,
      ~notFocused=?,
      ~useTextArea=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    let thereIsText = value !== "";

    editing ?
      <EditableEditingState
        editingToggle
        ?style
        ?className
        value
        placeholder
        onTextChange
        ?onKeyDown
        ?useTextArea
        ?notFocused
      /> :
      <div
        style={Utils.Dom.combineStyles(style, textStyle(thereIsText))}
        ?className
        onClick={_e => editingToggle() |> ignore}>
        {(thereIsText ? value : placeholder) |> Utils.String.stringToDivHtml}
      </div>;
  },
};