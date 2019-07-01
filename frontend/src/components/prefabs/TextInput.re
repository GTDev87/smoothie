let component = ReasonReact.statelessComponent("TextInput");

let make =
    (
      ~style=?,
      ~className=?,
      ~value,
      ~onTextChange,
      ~onKeyDown=?,
      ~placeholder,
      ~onBlur=?,
      ~autoFocus=?,
      _children,
    ) => {
  ...component,
  render: _self =>
    <GenericInput
      isTextArea=false
      ?className
      ?style
      value
      onTextChange
      ?onKeyDown
      placeholder
      ?onBlur
      ?autoFocus
    />,
};