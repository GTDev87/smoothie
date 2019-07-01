let component = ReasonReact.statelessComponent("MemberLayout");

let make = _children => {
  ...component,
  render: _self =>
    <Member.Container errorComponent={<LoginLayout />}>
      ...{
           (~member) =>
             <User.Container id=member##id>
               ...{(user) => <UserLayout user />}
             </User.Container>
         }
    </Member.Container>,
};