let component = ReasonReact.statelessComponent("ContentHeader");

let baseHeaderStyles =
  ReactDOMRe.Style.make(
    ~padding=".3em 0",
    ~marginBottom="1em",
    ~width="100%",
    ~borderBottom="solid 1px gray",
    (),
  );

let headerStyle = style =>
  switch (style) {
  | Some(style) => ReactDOMRe.Style.combine(baseHeaderStyles, style)
  | None => baseHeaderStyles
  };

let make = (~style=?, children) => {
  ...component,
  render: _self =>
    ReasonReact.createDomElement(
      "div",
      ~props={"style": headerStyle(style)},
      children,
    ),
};