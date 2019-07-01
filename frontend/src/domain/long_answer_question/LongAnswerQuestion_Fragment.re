module GraphFragment = [%graphql
  {|
    fragment longAnswerQuestionFields on LongAnswerQuestion {
      id
      questionBaseId
      questionBase {
        ...QuestionBase.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.LongAnswerQuestionFields;

let fragmentType = "LongAnswerQuestion";