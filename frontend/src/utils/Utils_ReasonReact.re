let listToReactArray = (list) =>
  list
  |> Belt.List.toArray
  |> ReasonReact.arrayToElement;

let createBlankComponent = (name) =>
  {...ReasonReact.statelessComponent(name), render: (_self) => <div/>};