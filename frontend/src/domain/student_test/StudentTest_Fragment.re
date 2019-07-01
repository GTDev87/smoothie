module GraphFragment = [%graphql
  {|
    fragment studentTestFields on StudentTest {
      id
      questionIds
      questions{
        ...StudentQuestion.Model.Fragment.Fields
      }
      originalId
      original{
        ...Test.Model.Fragment.TestFields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.StudentTestFields;

let fragmentType = "StudentTest";