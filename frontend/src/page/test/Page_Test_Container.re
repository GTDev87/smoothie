let component = ReasonReact.statelessComponent("Page_Test_Container");

module Container = ApolloQuery.Container(Query.Test.M);

let getFirstIdFromArray = (array: array('a), fn: 'a => 'b): option('b) =>
  array |> Belt.List.fromArray |> Belt.List.head |> Belt.Option.map(_, fn);
let make = (~testId: string, ~errorComponent, children) => {
  ...component,
  render: _ => {
    let testQuery = Query.Test.M.make(~testIds=[|testId|], ());
    <Container query=testQuery errorComponent>
      ...{
           (~response) =>
             children(
               ~testId=
                 response##tests
                 |> getFirstIdFromArray(
                      _, (test: Test.Model.Fragment.Fields.t) =>
                      test##id
                    ),
             )
         }
    </Container>;
  },
};