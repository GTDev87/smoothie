let component = ReasonReact.statelessComponent("RosterLayout");

let textStyle = ReactDOMRe.Style.make(~margin="1em", ());

let fullsizeStyle = ReactDOMRe.Style.make(~height="100%", ~width="100%", ());

let classNameTextStyle =
  ReactDOMRe.Style.make(~width="100%", ~borderBottom="solid 1px gray", ());

let make =
    (
      ~classroom: Classroom.Model.Record.t,
      ~testIds : list(Test.Model.idType),
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self => {
    let selectedTest =
      switch (classroom.local.testTab) {
      | None => testIds |> Belt.List.head
      | a => a
      };

    <InfoCard header=(<RosterLayoutHeader data=classroom testIds normalized updateNormalizr />)>
      {
        classroom.data.studentIds
        |> Belt.List.map(_, studentId => {
          let student =
            normalized
            |> MyNormalizr.Converter.Student.Remote.getRecord(_, studentId)
            |> Belt.Option.getWithDefault(_, Student.Model.Record.default());

          let updateStudent = action =>
            MyNormalizr.Converter.Student.Remote.updateWithDefault(
              (), normalized |> Js.Promise.resolve, Schema.Student.stringToId(student.data.id), action)
            |> updateNormalizr;

          <StudentRow
            key={student.data.id}
            classroom
            student
            updateStudent
            selectedTest
          />;
        })
        |> Utils.ReasonReact.listToReactArray
      }
    </InfoCard>;
  },
};