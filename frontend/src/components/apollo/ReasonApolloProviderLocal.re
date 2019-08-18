[@bs.config {jsx: 3}];

[@react.component]
let make = (~client, ~children) => {
  <ReasonApollo.Provider client>
    children
  </ReasonApollo.Provider>
};

module Jsx2 = {
  let component = ReasonReact.statelessComponent("ReasonApolloProviderLocal");
  /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
  let make = (~client, children) => {
    let children = React.array(children);
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~client, ~children, ()),
      children,
    );
  }
};