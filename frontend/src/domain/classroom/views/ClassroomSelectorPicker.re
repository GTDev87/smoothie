let component = ReasonReact.statelessComponent("ClassroomSelectorPicker");

let fullWidthStyle =
  ReactDOMRe.Style.make(
    ~width="content-fit",
    ~display="flex",
    ~justifyContent="flex-start",
    (),
  );

let selectorStyle =
  ReactDOMRe.Style.make(~marginRight="1em", ~display="flex", ());

let textStyle = ReactDOMRe.Style.make(~marginRight="1em", ());

let make =
    (
      ~classroom: Classroom.Model.Record.t,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~showStudentSelector,
      ~testIds : list(Test.Model.idType),
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

    let updateClassroom = action =>
      MyNormalizr.Converter.Classroom.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        Schema.Classroom.stringToId(classroom.data.id),
        action,
      )
      |> updateNormalizr;

    <div style=fullWidthStyle>
      <div style=selectorStyle>
        <div style=textStyle> {ReasonReact.string("Test:")} </div>
        <div>
          <Select
            selectedId=selectedTest
            idToString={a => a |> Belt.Option.mapWithDefault(_, "", (testId) => testId |> Test.Model.getUUIDFromId)}
            selections={
              testIds
              |> MyNormalizr.Converter.Test.idListToFilteredItems(_, MyNormalizr.Converter.Test.Remote.getRecord(normalized))
              |> Belt.List.map(_, test =>
                   (
                     {
                       id: Some(test.data.id |> Test.Model.idToTypedId),
                       text: test.data.name
                     }: Select.selectionType(option(Test.Model.idType))
                   )
                 )
            }
            onSelect={
              a =>
                switch(a){
                | Some(Some(a)) => updateClassroom(Classroom.Action.ChangeTestTab(a))
                | _ => updateClassroom(Classroom.Action.NoOpKeyDown)
                }
            }
          />
        </div>
      </div>
      {
        showStudentSelector ?
          <div style=selectorStyle>
            <div style=textStyle> {ReasonReact.string("Student:")} </div>
            <div>
              <Select
                selectedId=selectedStudent
                idToString={a => a}
                selections={
                  classroom.data.studentIds
                  |> MyNormalizr.Converter.Student.idListToFilteredItems(
                       _,
                       MyNormalizr.Converter.Student.Remote.getRecord(
                         normalized,
                       ),
                     )
                  |> Belt.List.map(_, student =>
                       (
                         {id: student.data.id, text: student.data.firstName}:
                           Select.selectionType(UUID.t)
                       )
                     )
                }
                onSelect={
                  a =>
                    updateClassroom(
                      Classroom.Action.SelectStudent(
                        Some(Schema.Student.stringToId(Belt.Option.getWithDefault(a, ""))),
                      ),
                    )
                }
              />
            </div>
          </div> :
          <div />
      }
    </div>;
  },
};