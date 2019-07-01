let component = ReasonReact.statelessComponent("LoginBackground");

let loginPageStyle =
  ReactDOMRe.Style.make(
    ~minHeight="100%",
    ~width="100%",
    ~background=
      "linear-gradient(
      "
      ++ Colors.primaryWithOpacity
      ++ ",
      "
      ++ Colors.primaryWithOpacity
      ++ "
    ),
    url('login-image.png')",
    ~backgroundPosition="center",
    ~backgroundRepeat="no-repeat",
    ~backgroundSize="cover",
    (),
  );

let make = (~style=?, children) => {
  ...component,
  render: _self =>
    ReasonReact.createDomElement(
      "div",
      ~props={"style": Utils.Dom.combineStyles(style, loginPageStyle)},
      children,
    ),
};