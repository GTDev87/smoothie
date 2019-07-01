/* not can't put the following into a union and pass around */
module GraphFragment = [%graphql
  {|
    fragment trueFalseQuestionFields on TrueFalseQuestion {
      id
      questionBaseId
      questionBase{
        ...QuestionBase.Model.Fragment.Fields
      }
      multipleChoice{
        ...MultipleChoice.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.TrueFalseQuestionFields;

let fragmentType = "TrueFalseQuestion";