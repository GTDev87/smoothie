module GraphFragment = [%graphql
  {|
    fragment studentAnswerKeyFields on StudentAnswerKey {
      id
      correct
      originalId
      original{
        ...QuestionAnswerKey.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.StudentAnswerKeyFields;

let fragmentType = "StudentAnswerKey";