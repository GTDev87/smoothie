let component = ReasonReact.statelessComponent("TiroLogo");

let flexCenter =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="center", ());

let logoStyle = ReactDOMRe.Style.make(~height="3em", ~margin="1em 0", ());

let make = (~style=?, _children) => {
  ...component,
  render: _self =>
    <div ?style>
      <div style=flexCenter>
        <img src="/logo-tiro.svg" style=logoStyle />
      </div>
    </div>,
};