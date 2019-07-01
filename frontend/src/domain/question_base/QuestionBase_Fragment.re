/* not can't put the following into a union and pass around */
module GraphFragment = [%graphql
  {|
    fragment questionBaseFields on QuestionBase {
      id
      text
      solution
      autoScore
      forcedResponse
      stimulusId
      questionType
      questionNumber
      stimulus{
        ...Stimulus.Model.Fragment.Fields
      }
      answerKeyIds
      answerKeys{
        ...QuestionAnswerKey.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.QuestionBaseFields;

let fragmentType = "QuestionBase";