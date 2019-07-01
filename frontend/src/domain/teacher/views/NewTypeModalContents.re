let component = ReasonReact.statelessComponent("NewTypeModalContents");

let inputStyle = ReactDOMRe.Style.make(~textAlign="center", ());

let make =
    (
      ~titleText,
      ~descriptionText,
      ~buttonText,
      ~addType,
      ~placeholderText,
      ~value,
      ~onChange,
      ~onKeyDown,
      _children,
    ) => {
  ...component,
  render: _self =>
    <NewTypeModalContentLayout titleText descriptionText buttonText addType>
      <TextInput
        style=inputStyle
        placeholder=placeholderText
        value
        onTextChange={a => onChange(a)}
        onKeyDown
        autoFocus=true
      />
    </NewTypeModalContentLayout>,
};