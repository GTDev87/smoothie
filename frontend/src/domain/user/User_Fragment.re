module GraphFragment = [%graphql
  {|
    fragment userFields on User {
      id
      email
      teacherId
      teacher {
        ...Teacher.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.UserFields;

let fragmentType = "User";