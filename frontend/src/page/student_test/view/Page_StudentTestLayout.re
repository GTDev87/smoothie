let component = ReasonReact.statelessComponent("Page_StudentTestLayout");

let make =
    (~classroomId: string, ~studentId: string, ~testId: string, _children) => {
  ...component,
  render: _self =>
    <Page_StudentTest_Container
      classroomId studentId testId errorComponent={<LoginLayout />}>
      ...{
           (~classroomId, ~testId, ~studentId) =>
             switch (classroomId, testId, studentId) {
             | (Some(classroomId), Some(testId), Some(studentId)) =>
               <Classroom.Container id=classroomId>
                 ...(
                      (classroom) =>
                        <Student.Container id=studentId>
                          ...{
                               (student) =>
                                 <Test.Container id=testId>
                                   ...{
                                        (test) =>
                                          <NormalizrInit
                                            records=[
                                              Classroom.Record.Record(classroom),
                                              Student.Record.Record(student),
                                              Test.Record.Record(test),
                                            ]>
                                            ...{
                                                 (
                                                   ~normalized,
                                                   ~updateNormalizr,
                                                 ) =>
                                                   <StudentTakeTest
                                                     classroomId
                                                     testId
                                                     studentId
                                                     normalized
                                                     updateNormalizr
                                                   />
                                               }
                                          </NormalizrInit>
                                      }
                                 </Test.Container>
                             }
                        </Student.Container>
                    )
               </Classroom.Container>

             | _ => <div />
             }
         }
    </Page_StudentTest_Container>,
};