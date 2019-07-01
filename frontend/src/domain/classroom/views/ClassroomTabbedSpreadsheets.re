let component = ReasonReact.statelessComponent("ClassroomTabbedSpreadsheets");

let fullsizeStyle = ReactDOMRe.Style.make(~height="100%", ~width="100%", ());

let make =
    (
      ~classroom: Classroom.Model.Record.t,
      ~normalized,
      ~updateNormalizr: Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self => {
    let selectedTab =
      switch (classroom.local.testTab) {
      | None => classroom.data.testIds |> Belt.List.head
      | a => a
      };

    let students = MyNormalizr.Converter.Student.idListToFilteredItems(classroom.data.studentIds, MyNormalizr.Converter.Student.Remote.getRecord(normalized));

    <div style=fullsizeStyle>
      {
        classroom.data.testAnalyticsIds
        |> MyNormalizr.Converter.TestAnalytics.idListToFilteredItems(_, MyNormalizr.Converter.TestAnalytics.Remote.getRecord(normalized))
        |> Belt.List.keep(_, testAnalytics => Some(testAnalytics.data.testId) == selectedTab)
        |> Belt.List.head
        |> Belt.Option.mapWithDefault(_, <div/>, (selectedTestAnalytics) => {
          normalized
          |> MyNormalizr.Converter.Test.Remote.getRecord(_, selectedTestAnalytics.data.testId)
          |> Belt.Option.mapWithDefault(_, <div/>, (selectedTest) => {
            <TestTabbedSpreadsheets
              headerSection={
                <ClassroomSelectorPicker
                  classroom
                  normalized
                  updateNormalizr
                  testIds={classroom.data.testIds}
                  showStudentSelector=false
                />
              }
              selectedTestAnalytics
              normalized
              students
              updateTest=(
                action =>
                  MyNormalizr.Converter.Test.Remote.updateWithDefault((), normalized |> Js.Promise.resolve, Schema.Test.stringToId(selectedTest.data.id), action)
                  |> updateNormalizr
              )
            />
          })
        })
      }
    </div>
  },
};