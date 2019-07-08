module GraphFragment = [%graphql
  {|
    fragment profileFields on Profile {
      id
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.ProfileFields;

let fragmentType = "Profile";