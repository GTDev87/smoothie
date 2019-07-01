let component = ReasonReact.statelessComponent("NewClassroomModalContents");

let newContentStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~color="gray",
    ~marginTop="3em",
    (),
  );

let flexCenter =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="center", ());

let inputContentStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~marginBottom="2em",
    (),
  );

let inputStyle = ReactDOMRe.Style.make(~textAlign="center", ());

let h1Style = ReactDOMRe.Style.make(~color="gray", ~marginBottom="1em", ());

let descriptionTextStyle =
  ReactDOMRe.Style.make(~fontSize=".7em", ~textAlign="center", ());

let make =
    (
      ~newClassroom: Classroom.Model.Record.t,
      ~addNewClassroom,
      ~updateNewClassroom,
      ~normalized,
      _children,
    ) => {
  ...component,
  render: _self =>
    <NewTypeModalContents
      titleText="Create a classroom"
      placeholderText="Classroom name"
      descriptionText="Classrooms are where you store your student rosters, where you see tests completed by students and where you see your analytics results from taken tests."
      buttonText="Create class"
      value={newClassroom.data.name}
      onChange={a => updateNewClassroom(Classroom.Action.EditName(a))}
      addType=addNewClassroom
      onKeyDown={
        GenericInput.enterSubmitWithCommand(addNewClassroom, () =>
          updateNewClassroom(Classroom.Action.NoOpKeyDown)
        )
      }
    />,
};