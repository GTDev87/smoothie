/* This doesn't really belong here but I'll add it it belongs with queries*/
let component = ReasonReact.statelessComponent("MemberContainer");

module Container = ApolloQuery.Container(Query.Member.M);

let make = (~errorComponent, children) => {
  ...component,
  render: _ =>
    <Container query={Query.Member.M.make()} errorComponent>
      ...{(~response) => children(~member=response##member)}
    </Container>,
};