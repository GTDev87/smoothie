/* not can't put the following into a union and pass around */
module GraphFragment = [%graphql
  {|
    fragment multipleChoiceQuestionFields on MultipleChoiceQuestion {
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
module Fields = GraphFragment.MultipleChoiceQuestionFields;

let fragmentType = "MultipleChoiceQuestion";