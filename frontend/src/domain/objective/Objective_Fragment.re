module GraphFragment = [%graphql
  {|
    fragment objectiveFields on Objective {
      id
      text
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.ObjectiveFields;

let fragmentType = "Objective";