let component = ReasonReact.statelessComponent("ApolloQueryContainer");

module Container = (Config: ReasonApolloTypes.Config) => {
  module QueryComponent = ReasonApollo.CreateQuery(Config);

  let defaultLoadingComponent = <div> {ReasonReact.string("Loading")} </div>;
  let defaultErrorComponent = error =>
    <div>
      {ReasonReact.string("Error: ")}
      {ReasonReact.string(error##message)}
    </div>;

  let make = (~query, ~errorComponent=?, ~loadingComponent=?, children) => {
    ...component,
    render: _ =>
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
      </QueryComponent>,
  };
};