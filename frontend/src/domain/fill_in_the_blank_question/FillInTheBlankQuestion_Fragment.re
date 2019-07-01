/* not can't put the following into a union and pass around */
module GraphFragment = [%graphql
  {|
    fragment fillInTheBlankQuestionFields on FillInTheBlankQuestion {
      id
      questionBaseId
      questionBase {
        ...QuestionBase.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.FillInTheBlankQuestionFields;

let fragmentType = "FillInTheBlankQuestion";
