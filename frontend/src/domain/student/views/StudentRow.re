let component = ReasonReact.statelessComponent("StudentRow");

let fullWidthStyle =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~margin="0",
    ~backgroundColor="white",
    ~borderBottom="solid 1px " ++ Colors.secondary, /* what color should this be? */
    ~marginBottom="0.2em",
    ~padding=".5em",
    (),
  );

let css = Css.css;
let tw = Css.tw;

let linkClass = [%bs.raw {| css(tw` w-full whitespace-no-wrap`)|}];

let wrapperLinkClass = [%bs.raw {| css(tw` w-1/6 overflow-scroll`)|}];

let rowClass = [%bs.raw
  {| css(tw`flex flex-wrap w-full bg-white border-0 border-b border-blue-lighter border-solid `)|}
];
let sixthWidthClass = [%bs.raw {| css(tw` w-1/6`)|}];

let make =
    (
      ~student: Student.Model.Record.t,
      ~classroom: Classroom.Model.Record.t,
      ~updateStudent,
      ~selectedTest,
      _children,
    ) => {
  ...component,
  render: _self => {
    let firstNameOnKeyDown =
      GenericInput.enterSubmitWithCommand(
        () => updateStudent(Student.Action.ToggleEditFirstName),
        () => updateStudent(Student.Action.NoOp),
      );

    let lastNameOnKeyDown =
      GenericInput.enterSubmitWithCommand(
        () => updateStudent(Student.Action.ToggleEditLastName),
        () => updateStudent(Student.Action.NoOp),
      );

    let link =
      switch(selectedTest){
      | None => ""
      | Some(testId) => 
          Document.origin
            ++ "/classroom/"
            ++ classroom.data.id
            ++ "/student/"
            ++ student.data.id
            ++ "/test/"
            ++ Test.Model.getUUIDFromId(testId)
      };
        

    <Student.Mutation.UpdateStudent id={student.data.id}>
      ...{(~apolloUpdateStudent) =>
        <div className=rowClass>
          <div className=sixthWidthClass />
          <div className=sixthWidthClass>
            <Editable
              editing={student.local.edittingFirstName}
              editingToggle={
                () => updateStudent(Student.Action.ToggleEditFirstName)
              }
              value={student.data.firstName}
              placeholder="First Name"
              onTextChange={
                text =>
                  updateStudent(
                    Student.Action.ApolloUpdateStudent(
                      () =>
                        apolloUpdateStudent(
                          ~lastName=student.data.lastName,
                          ~firstName=text,
                          ~gradeId=student.data.gradeId,
                        ),
                    ),
                  )
              }
              onKeyDown=firstNameOnKeyDown
            />
          </div>
          <div className=sixthWidthClass>
            <Editable
              editing={student.local.edittingLastName}
              editingToggle={
                () => updateStudent(Student.Action.ToggleEditLastName)
              }
              value={student.data.lastName}
              placeholder="Last Name"
              onTextChange={
                text =>
                  updateStudent(
                    Student.Action.ApolloUpdateStudent(
                      () =>
                        apolloUpdateStudent(
                          ~lastName=text,
                          ~firstName=student.data.firstName,
                          ~gradeId=student.data.gradeId,
                        ),
                    ),
                  )
              }
              onKeyDown=lastNameOnKeyDown
              notFocused=true
            />
          </div>
          <div className=sixthWidthClass>
            <Grades_Container>
              ...{
                  (~grades) => {
                    let gradeRecords =
                      grades
                      |> Belt.List.fromArray
                      |> Belt.List.map(_, Grade.Model.Record.fromObject);

                    let gradeToString = id =>
                      gradeRecords
                      |> Belt.List.keep(_, g => g.data.id == id)
                      |> Belt.List.head
                      |> Belt.Option.mapWithDefault(_, "blank", g =>
                            g.data.name
                          );

                    <Select
                      selectedId={student.data.gradeId}
                      idToString=gradeToString
                      selections={
                        gradeRecords
                        |> Belt.List.map(
                              _, (grade: Grade.Model.Record.t) =>
                              (
                                {id: grade.data.id, text: grade.data.name}:
                                  Select.selectionType(UUID.t)
                              )
                            )
                      }
                      onSelect={
                        grade =>
                          updateStudent(
                            Student.Action.ApolloUpdateStudent(
                              () =>
                                apolloUpdateStudent(
                                  ~lastName=student.data.lastName,
                                  ~firstName=student.data.firstName,
                                  ~gradeId=
                                    Belt.Option.getWithDefault(
                                      grade,
                                      "blank",
                                    ),
                                ),
                            ),
                          )
                      }
                    />;
                  }
                }
            </Grades_Container>
          </div>
          <div className=wrapperLinkClass>
            <a href=link target="_blank" className=linkClass>
              {ReasonReact.string(link)}
            </a>
          </div>
          <div className=sixthWidthClass />
        </div>
      }
    </Student.Mutation.UpdateStudent>;
  },
};