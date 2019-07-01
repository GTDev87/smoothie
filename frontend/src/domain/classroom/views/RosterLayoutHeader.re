let component = ReasonReact.statelessComponent("RosterLayoutHeader");
let css = Css.css;
let tw = Css.tw;
let testRowHeaderClass = [%bs.raw
  {| css(tw`
    flex
  `)|}
];


let make = (
  ~data as classroom : Classroom.Model.Record.t,
  ~testIds,
  ~normalized,
  ~updateNormalizr,
  _childre
) => {
  ...component,
  render: _self => {
    let updateClassroom = action =>
      MyNormalizr.Converter.Classroom.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        Schema.Classroom.stringToId(classroom.data.id),
        action,
      )
      |> updateNormalizr;

    let studentHasEmptyField =
      classroom.data.studentIds
      |> MyNormalizr.Converter.Student.idListToFilteredItems(
           _,
           MyNormalizr.Converter.Student.Remote.getRecord(normalized),
         )
      |> Belt.List.keep(_, student =>
           student.data.firstName === "" && student.data.lastName === ""
         )
      |> Belt.List.length;

    <Classroom.Mutation.AddStudent
      id={classroom.local.newStudentId |> Student.Model.getUUIDFromId}
      classroomId={classroom.data.id}>
      ...{(~addStudent) =>
        <div>
          <StudentRowHeader
            normalized
            classroom
            testIds
            updateNormalizr
            addingNewStudent={studentHasEmptyField > 0}
            addStudent={
              _ =>
                updateClassroom(
                  Classroom.Action.ApolloAddStudent(addStudent),
                )
                |> ignore
            }
          />
        </div>
      }
    </Classroom.Mutation.AddStudent>
  }
};