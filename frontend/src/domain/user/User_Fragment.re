module GraphFragment = [%graphql
  {|
    fragment userFields on User {
      id
      email
      profile {
        ...Profile.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.UserFields;

let fragmentType = "User";