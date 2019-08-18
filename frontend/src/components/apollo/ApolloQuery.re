[@bs.config {jsx: 3}];
let component = ReasonReact.statelessComponent("ApolloQueryContainer");

module Container = (Config: ReasonApolloTypes.Config) => {
  module QueryComponent = ReasonApollo.CreateQuery(Config);

  let defaultLoadingComponent = <div> {ReasonReact.string("Loading")} </div>;
  let defaultErrorComponent = error =>
    <div>
      {ReasonReact.string("Error: ")}
      {ReasonReact.string(error##message)}
    </div>;

  [@react.component]
  let make = (
    ~query,
    ~errorComponent,
    ~loadingComponent,
    ~children
  ) => {
    <QueryComponent variables=query##variables>
      ...{
            ({result}) =>
              switch (result) {
              | Loading =>
                loadingComponent
                |> Belt.Option.getWithDefault(_, defaultLoadingComponent)
              | Error(error) =>
                Belt.Option.getWithDefault(
                  errorComponent,
                  defaultErrorComponent(error),
                )
              | Data(response) => children(~response)
              }
          }
    </QueryComponent>
  };

  module Jsx2 = {
    let component = ReasonReact.statelessComponent("GradesContainer");
    /* `children` is not labelled, as it is a regular parameter in version 2 of JSX */
    let make = (
      ~query,
      ~errorComponent=?,
      ~loadingComponent=?,
      children
    ) =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~query, ~errorComponent, ~loadingComponent, ~children,()),
        [||],
      );
  };
};

