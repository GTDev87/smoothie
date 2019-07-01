let component = ReasonReact.statelessComponent("NewTestModalContents");

let flexCenter =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="center", ());

let make =
    (
      ~newTest: Test.Model.Record.t,
      ~addNewTest,
      ~updateNewTest,
      ~normalized,
      _children,
    ) => {
  ...component,
  render: _self =>
    <NewTypeModalContents
      titleText="Create a Test"
      placeholderText="Test name"
      descriptionText="Test are what you distribute to your students.  Analytics for test will be inside of the classrooms tab."
      buttonText="Create a Test"
      value={newTest.data.name}
      onChange={a => updateNewTest(Test.Action.EditName(a))}
      addType=addNewTest
      onKeyDown={
        GenericInput.enterSubmitWithCommand(addNewTest, () =>
          updateNewTest(Test.Action.NoOpKeyDown)
        )
      }
    />,
};