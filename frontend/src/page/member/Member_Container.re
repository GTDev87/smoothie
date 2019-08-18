/* This doesn't really belong here but I'll add it it belongs with queries*/
module Container = ApolloQuery.Container(Query.Member.M);

let component = ReasonReact.statelessComponent("MemberContainer");

let make = (~errorComponent, children) => {
  ...component,
  render: _ =>
    <Container.Jsx2 query={Query.Member.M.make()} errorComponent>
      ...{(~response) => children(~member=response##member)}
    </Container.Jsx2>
};