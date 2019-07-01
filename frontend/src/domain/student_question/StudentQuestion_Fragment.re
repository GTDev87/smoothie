module GraphFragment = [%graphql
  {|
    fragment studentQuestionFields on StudentQuestion {
      id
      answer
      originalId
      original {
        ...on LongAnswerQuestion {
          ...LongAnswerQuestion.Model.Fragment.Fields
        }
        ...on MultipleChoiceQuestion {
          ...MultipleChoiceQuestion.Model.Fragment.Fields
        }
        ...on TrueFalseQuestion {
          ...TrueFalseQuestion.Model.Fragment.Fields
        }
        ...on FillInTheBlankQuestion {
          ...FillInTheBlankQuestion.Model.Fragment.Fields
        }
      }
      answerKeyIds
      answerKeys{
        ...StudentAnswerKey.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.StudentQuestionFields;

let fragmentType = "StudentQuestion";