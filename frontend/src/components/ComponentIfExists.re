let component = ReasonReact.statelessComponent("ComponentIfExists");
let make = (~maybeData, ~render, _children) => {
  ...component,
  render:
    (_self) => Belt.Option.mapWithDefault(maybeData, <div />, render)
};
