let component = ReasonReact.statelessComponent("UserLayout");

let fullSizeStyle = ReactDOMRe.Style.make(~height="100%", ());

let make = (~user: User.Model.Record.t, _children) => {
  ...component,
  render: _self =>
    <div style=fullSizeStyle>
      {
        switch (user.data.profileId) {
        | None => <AccountTypeLayout />
        | Some(profileId) =>
          <Profile.Container id={Profile.Model.getUUIDFromId(profileId)}>
            ...((teacher) => {
              <NormalizrInit records=[Profile.Record.Record(teacher)]>
                ...{(~normalized, ~updateNormalizr) =>
                  <ProfileLayout  />
                }
              </NormalizrInit>
            })
          </Profile.Container>
        }
      }
    </div>,
};