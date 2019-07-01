module M = [%graphql
  {|
    query StudentTest($classroomIds: [ID!], $studentIds: [ID!], $testIds: [ID!]) {
      classrooms(filter: {ids: $classroomIds}) {
        ...Classroom.Model.Fragment.Fields
      }
      students(filter: {ids: $studentIds}) {
        ...Student.Model.Fragment.Fields
      }
      tests(filter: {ids: $testIds}) {
        ...Test.Model.Fragment.Fields
      }
    }
  |}
];