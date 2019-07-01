let component = ReasonReact.statelessComponent("Checkbox");

let make = (~value, ~onToggle, ~id=?, _children) => {
  ...component,
  render: _self =>
    <input
      id={Belt.Option.getWithDefault(id, "")}
      _type="checkbox"
      checked=value
      onChange={_e => onToggle() |> ignore}
    />,
};