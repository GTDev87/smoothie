module GraphFragment = [%graphql
  {|
     fragment gradeFields on Grade {
       id
       name
     }
   |}
];

include GraphFragment;
module Fields = GraphFragment.GradeFields;

let fragmentType = "Grade";