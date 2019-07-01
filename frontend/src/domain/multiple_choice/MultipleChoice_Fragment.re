module GraphFragment = [%graphql
  {|
    fragment multipleChoiceFields on MultipleChoice {
      id
      choiceIds
      choices {
        id
        text
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.MultipleChoiceFields;

let fragmentType = "MultipleChoice";