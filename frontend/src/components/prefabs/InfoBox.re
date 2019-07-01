let component = ReasonReact.statelessComponent("InfoBox");

let infoProps =
  ReactDOMRe.Style.make(
    ~padding=".25em 1.4em",
    ~color="white",
    ~backgroundColor=Colors.primary,
    ~borderRadius=".2em",
    (),
  );

let make = (~info, ~style=?, _children) => {
  ...component,
  render: _self =>
    <div style={Utils.Dom.combineStyles(style, infoProps)}>
      {ReasonReact.string(info)}
    </div>,
};