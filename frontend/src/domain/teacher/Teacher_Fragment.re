module GraphFragment = [%graphql
  {|
    fragment teacherFields on Teacher {
      id
      testIds
      tests{
        ...Test.Model.Fragment.Fields
      }
      classroomIds
      classrooms{
        ...Classroom.Model.Fragment.Fields
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.TeacherFields;

let fragmentType = "Teacher";