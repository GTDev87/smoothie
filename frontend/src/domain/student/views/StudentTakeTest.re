let component = ReasonReact.statelessComponent("StudentTakeTest");

let fullsizeStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~width="100%",
    ~display="flex",
    ~flexDirection="column",
    (),
  );

let selectionPickerStyle =
  ReactDOMRe.Style.make(~width="100%", ~marginBottom="1.5em", ());

let remainingStyle =
  ReactDOMRe.Style.make(~width="100%", ~flex="2", ~overflow="auto", ());

let make =
    (
      ~classroomId: string,
      ~testId: string,
      ~studentId: string,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self =>
    switch (
      MyNormalizr.Converter.Classroom.Remote.getRecord(
        normalized,
        Schema.Classroom.stringToId(classroomId),
      )
    ) {
    | None => <div />
    | Some(classroom) =>
      <Classroom.Mutation.GiveTestToClassroom>
        ...(
             (~mutation as giveTestToClassroomMutation) =>
               <div style=fullsizeStyle>
                 {
                   normalized
                   |> MyNormalizr.Converter.Student.Remote.getRecord(_, Schema.Student.stringToId(studentId))
                   |> Belt.Option.mapWithDefault(_, <div />, student =>
                        student.data.testIds
                        |> MyNormalizr.Converter.StudentTest.idListToFilteredItems(_, MyNormalizr.Converter.StudentTest.Remote.getRecord(normalized))
                        |> Belt.List.getBy(_, studentTest => studentTest.data.originalId == Schema.Test.stringToId(testId))
                        |> Belt.Option.getWithDefault(_, StudentTest.Model.Record.default(testId))
                        |> (studentTest =>
                            <div style=remainingStyle>
                              <Student.Mutation.GiveTestToStudent>
                                ...{
                                     (~giveTestToStudent) =>
                                       <StudentTestWriter
                                         normalized
                                         updateNormalizr
                                         updateNormalizedWithReducer={
                                           Student_Function.updateClassroomAndStudentNormalizr(
                                             normalized,
                                             giveTestToClassroomMutation,
                                             giveTestToStudent,
                                             classroom: Classroom.Model.Record.t,
                                             studentTest: StudentTest.Model.Record.t,
                                             student: Student.Model.Record.t,
                                           )
                                         }
                                         studentTest
                                       />
                                   }
                              </Student.Mutation.GiveTestToStudent>
                            </div>
                        )
                      )
                 }
               </div>
           )
      </Classroom.Mutation.GiveTestToClassroom>
    },
};