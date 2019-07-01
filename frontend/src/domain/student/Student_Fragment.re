module GraphFragment = [%graphql
  {|
    fragment studentFields on Student {
      id
      firstName
      lastName
      gradeId
      grade{
        id
      }
      testIds
      tests{
        ...StudentTest.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.StudentFields;

let fragmentType = "Student";