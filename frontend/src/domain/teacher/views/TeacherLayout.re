let component = ReasonReact.statelessComponent("TeacherLayout");

let fullWidth =
  ReactDOMRe.Style.make(~width="100%", ~padding="0", ~margin="0", ());

let css = Css.css;
let tw = Css.tw;

let teacherMainContentClass = [%bs.raw
  {| css(tw` w-full h-full pt-4 pl-8 `)|}
];

let flexCenter =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~alignItems="center",
    (),
  );

let textStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="space-between",
    ~paddingLeft=".5em",
    (),
  );

let widthStyle = "15%";

let sidebarStyle =
  ReactDOMRe.Style.make(
    ~width=widthStyle,
    ~backgroundColor=Colors.primary,
    ~padding="0",
    ~color="white",
    (),
  );

let realAndOpenStyle = ReactDOMRe.Style.make(~padding="1em", ());

let fullSizeStyle = ReactDOMRe.Style.make(~height="100%", ~width="100%", ());

let make =
    (
      ~teacherId,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~normalized: MyNormalizr.FullReduced.normalizedType,
      _children,
    ) => {
  let normalizeTeacher = (normalized, action) =>
    MyNormalizr.Converter.Teacher.Remote.updateWithDefault(
      (),
      normalized,
      teacherId,
      action,
    );

  let updateTeacher = action =>
    normalizeTeacher(normalized |> Js.Promise.resolve, action)
    |> updateNormalizr;

  ComponentIfExists.make(
    ~maybeData=
      MyNormalizr.Converter.Teacher.Remote.getRecord(normalized, teacherId),
    ~render=
      teacher => {
        let updateNewClassroom = action =>
          MyNormalizr.Converter.Classroom.Remote.updateWithDefault(
            (),
            normalized |> Js.Promise.resolve,
            teacher.local.newClassroomId,
            action,
          )
          |> updateNormalizr;
        let updateNewTest = action =>
          MyNormalizr.Converter.Test.Remote.updateWithDefault(
            (),
            normalized |> Js.Promise.resolve,
            teacher.local.newTestId,
            action,
          )
          |> updateNormalizr;

        let tupleSelection = (
          teacher.local.sidebarSelection,
          teacher.local.subSidebarSelection,
        );

        let getIsDropdownOpen = selectionName =>
          teacher.local.openDropdown
          |> Belt.List.getBy(_, ((sectionName, _)) =>
               selectionName == sectionName
             )
          |> Belt.Option.getWithDefault(_, (SideTab.Dashboard, false))
          |> (((_, isOpen)) => isOpen);

        let isClassroomOpen = getIsDropdownOpen(SideTab.Classroom);
        let isTestOpen = getIsDropdownOpen(SideTab.Test);

        <div style=fullSizeStyle>
          <ReactModal
            autoFocus=false
            isOpen={teacher.local.openModal !== None}
            onRequestClose={() => updateTeacher(Teacher.Action.OpenModal(None))}
          >
            {
              switch (teacher.local.openModal) {
              | Some(Classroom) =>
                let newClassroom =
                  normalized
                  |> MyNormalizr.Converter.Classroom.Remote.getRecord(_, teacher.local.newClassroomId)
                  |> Belt.Option.getWithDefault(_, Classroom.Model.Record.defaultWithId((), teacher.local.newClassroomId));

                <Teacher.Mutation.AddClassroom
                  id={newClassroom.data.id}
                  name={newClassroom.data.name}
                  teacherId={teacherId |> Teacher.Model.getUUIDFromId}>
                  ...((~addClassroom) =>
                      <NewClassroomModalContents
                        newClassroom
                        addNewClassroom={
                          () =>
                            updateTeacher(
                              Teacher.Action.(
                                CombineReducer(
                                  /* AddClassroom, */
                                  ApolloAddClassroom(addClassroom),
                                  CombineReducer(
                                    OpenModal(None),
                                    SelectSideBar(
                                      SideTab.Classroom,
                                      teacher.local.newClassroomId
                                      |> Classroom.Model.getUUIDFromId,
                                    ),
                                  ),
                                )
                              ),
                            )
                        }
                        updateNewClassroom
                        normalized
                      />
                  )
                </Teacher.Mutation.AddClassroom>;

              | Some(Test) =>
                let newTest =
                  normalized
                  |> MyNormalizr.Converter.Test.Remote.getRecord(_, teacher.local.newTestId)
                  |> Belt.Option.getWithDefault(_, Test.Model.Record.defaultWithId((), teacher.local.newTestId));

                <Teacher.Mutation.AddTest
                  id={newTest.data.id}
                  name={newTest.data.name}
                  description={newTest.data.description}
                  teacherId={teacherId |> Teacher.Model.getUUIDFromId}>
                  ...((~addTest) =>
                    <NewTestModalContents
                      newTest
                      addNewTest={
                        () =>
                          updateTeacher(
                            Teacher.Action.(
                              CombineReducer(
                                ApolloAddTest(addTest),
                                CombineReducer(
                                  OpenModal(None),
                                  SelectSideBar(
                                    SideTab.Test,
                                    teacher.local.newTestId
                                    |> Test.Model.getUUIDFromId,
                                  ),
                                ),
                              )
                            ),
                          )
                      }
                      updateNewTest
                      normalized
                    />
                  )
                </Teacher.Mutation.AddTest>;
              | _ => <div />
              }
            }
          </ReactModal>
          <ReasonSidebar
            style=sidebarStyle
            sidebar={
              <div style=fullSizeStyle>
                <TiroLogo style=realAndOpenStyle />
                <div style=fullWidth>
                  <SideNav
                    highlightColor="white"
                    highlightBgColor=Colors.sideBarHighlight
                    tupleSelected=(
                      fst(tupleSelection) |> SideTab.toString,
                      snd(tupleSelection),
                    )
                    tupleOnItemSelection={
                      (selectionString, subSelection) =>
                        switch (
                          selectionString |> SideTab.fromString,
                          subSelection,
                        ) {
                        | (SideTab.Classroom, "new") =>
                          updateTeacher(Teacher.Action.OpenModal(Some(Classroom)))
                        | (SideTab.Test, "new") =>
                          updateTeacher(Teacher.Action.OpenModal(Some(Test)))
                        | (SideTab.Dashboard, _) =>
                          updateTeacher(
                            Teacher.Action.SelectSideBar(
                              SideTab.Dashboard,
                              subSelection,
                            ),
                          )
                        | (sideBarType, "") =>
                          updateTeacher(Teacher.Action.ToggleDropdown(sideBarType))
                        | (sideBarType, _) =>
                          updateTeacher(
                            Teacher.Action.SelectSideBar(
                              sideBarType,
                              subSelection,
                            ),
                          )
                        }
                    }>
                    <SideNavNav tupleId=("dashboard", "")>
                      <SideNavNavText style=textStyle>
                        {ReasonReact.string("Planbook  ")}
                      </SideNavNavText>
                    </SideNavNav>
                    <SideNavNav tupleId=("classroom", "")>
                      <SideNavNavText style=textStyle>
                        {ReasonReact.string("Classes ")}
                        {isClassroomOpen ? <ChevronUpIcon /> : <ChevronDownIcon />}
                      </SideNavNavText>
                    </SideNavNav>
                    {
                      isClassroomOpen ?
                        (
                          teacher.data.classroomIds
                          |> MyNormalizr.Converter.Classroom.idListToFilteredItems(_, MyNormalizr.Converter.Classroom.Remote.getRecord(normalized))
                          |> Belt.List.map(_, classroom =>
                              <SideNavNav
                                tupleId=("classroom", classroom.data.id)
                                key={classroom.data.id}
                              >
                                <SideNavNavText style=textStyle>
                                  {ReasonReact.string(classroom.data.name)}
                                </SideNavNavText>
                              </SideNavNav>
                            )
                        )
                        @ [
                          <SideNavNav tupleId=("classroom", "new") key="new">
                            <SideNavNavText style=textStyle>
                              {ReasonReact.string("+ Add Class")}
                            </SideNavNavText>
                          </SideNavNav>,
                        ]
                        |> Utils.ReasonReact.listToReactArray :
                        <div />
                    }
                    <SideNavNav tupleId=("test", "")>
                      <SideNavNavText style=textStyle>
                        {ReasonReact.string("Test  ")}
                        {isTestOpen ? <ChevronUpIcon /> : <ChevronDownIcon />}
                      </SideNavNavText>
                    </SideNavNav>
                    {
                      isTestOpen ?
                        (
                          teacher.data.testIds
                          |> MyNormalizr.Converter.Test.idListToFilteredItems(
                               _, MyNormalizr.Converter.Test.Remote.getRecord(normalized))
                          |> Belt.List.map(_, (test: Test.Model.Record.t) =>
                               <SideNavNav
                                 tupleId=("test", test.data.id)
                                 key={test.data.id}>
                                 <SideNavNavText style=textStyle>
                                   {ReasonReact.string(test.data.name)}
                                 </SideNavNavText>
                               </SideNavNav>
                             )
                        )
                        @ [
                          <SideNavNav tupleId=("test", "new") key="test-new">
                            <SideNavNavText style=textStyle>
                              {ReasonReact.string("+ Add Test")}
                            </SideNavNavText>
                          </SideNavNav>,
                        ]
                        |> Utils.ReasonReact.listToReactArray :
                        <div />
                    }
                  </SideNav>
                </div>
              </div>
            }
            header={<div />}>
            <div className=teacherMainContentClass>
              {
                switch (tupleSelection) {
                | (SideTab.Classroom, _) =>
                  <ClassroomLayout
                    testIds={teacher.data.testIds}
                    normalized
                    updateNormalizr
                    classroomId={
                      Schema.Classroom.stringToId(
                        teacher.local.subSidebarSelection,
                      )
                    }
                  />
                | (SideTab.Test, _) =>
                  <TestBuilder
                    normalized
                    updateNormalizr
                    testId={Schema.Test.stringToId(teacher.local.subSidebarSelection)}
                  />
                | _ => <PlanbookLayout teacher normalized updateNormalizr />
                }
              }
            </div>
          </ReasonSidebar>
        </div>;
      },
    [||],
  );
};