let component = ReasonReact.statelessComponent("TakeTest");

let css = Css.css;
let tw = Css.tw;

let fullsizeStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~width="100%",
    ~display="flex",
    ~flexDirection="column",
    (),
  );

let selectionPickerClass = [%bs.raw
  {| css(tw`
    pb-2
  `)|}
];

let remainingStyle =
  ReactDOMRe.Style.make(~width="100%", ~flex="2", ~overflow="auto", ());

let make =
    (
      ~classroom: Classroom.Model.Record.t,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~testIds,
      _children,
    ) => {
  ...component,
  render: _self => {
    let selectedTest =
      switch (classroom.local.testTab) {
      | None => testIds |> Belt.List.head
      | a => a
      };

    let selectedStudent =
      switch (classroom.local.selectedStudent) {
      | None =>
        classroom.data.studentIds
        |> Belt.List.head
        |> Belt.Option.mapWithDefault(_, "", Student.Model.getUUIDFromId)
      | Some(a) =>
        MyNormalizr.Converter.Student.Remote.getRecord(normalized, a)
        |> Belt.Option.mapWithDefault(_, "", student => student.data.id)
      };
    <Classroom.Mutation.GiveTestToClassroom>
      ...{
           (~mutation as giveTestToClassroomMutation) =>
             <div style=fullsizeStyle>
               <div className=selectionPickerClass>
                 <ClassroomSelectorPicker
                   classroom
                   normalized
                   testIds
                   updateNormalizr
                   showStudentSelector=true
                 />
               </div>
               {
                 normalized
                 |> MyNormalizr.Converter.Student.Remote.getRecord(_, Schema.Student.stringToId(selectedStudent))
                 |> Belt.Option.mapWithDefault(_, <div />, student => {
                      testIds
                      |> Belt.List.getBy(_, testId => selectedTest == Some(testId))
                      |> Belt.Option.mapWithDefault(_, <div />, testId => {
                           student.data.testIds
                           |> MyNormalizr.Converter.StudentTest.idListToFilteredItems(_, MyNormalizr.Converter.StudentTest.Remote.getRecord(normalized))
                           |> Belt.List.getBy(_, studentTest => studentTest.data.originalId == testId)
                           |> Belt.Option.getWithDefault(_, StudentTest.Model.Record.default(testId |> Test.Model.getUUIDFromId))
                           |> (
                             studentTest =>
                               <div style=remainingStyle>
                                 <Student.Mutation.GiveTestToStudent>
                                   ...{
                                        (~giveTestToStudent) =>
                                          <TestWriter
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
                      })
                    })
               }
             </div>
         }
    </Classroom.Mutation.GiveTestToClassroom>;
  },
};