module GraphFragment = [%graphql
  {|
    fragment questionAnswerKeyFields on QuestionAnswerKey {
      id
      score
      objectiveId
      objective{
        ...Objective.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.QuestionAnswerKeyFields;

let fragmentType = "QuestionAnswerKey";