let component = ReasonReact.statelessComponent("Page_StudentTest_Container");

module Container = ApolloQuery.Container(Query.StudentTest.M);

let getFirstIdFromArray = (array: array('a), fn: 'a => 'b): option('b) =>
  array |> Belt.List.fromArray |> Belt.List.head |> Belt.Option.map(_, fn);
let make =
    (
      ~classroomId: string,
      ~studentId: string,
      ~testId: string,
      ~errorComponent,
      children,
    ) => {
  ...component,
  render: _ => {
    let studentTestQuery =
      Query.StudentTest.M.make(
        ~classroomIds=[|classroomId|],
        ~studentIds=[|studentId|],
        ~testIds=[|testId|],
        (),
      );
    <Container query=studentTestQuery errorComponent>
      ...{
           (~response) =>
             children(
               ~classroomId=
                 response##classrooms
                 |> getFirstIdFromArray(
                      _, (classroom: Classroom.Model.Fragment.Fields.t) =>
                      classroom##id
                    ),
               ~testId=
                 response##tests
                 |> getFirstIdFromArray(
                      _, (test: Test.Model.Fragment.Fields.t) =>
                      test##id
                    ),
               ~studentId=
                 response##students
                 |> getFirstIdFromArray(
                      _, (student: Student.Model.Fragment.Fields.t) =>
                      student##id
                    ),
             )
         }
    </Container>;
  },
};