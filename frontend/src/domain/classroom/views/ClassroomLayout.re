let component = ReasonReact.statelessComponent("ClassroomLayout");

let textStyle = ReactDOMRe.Style.make(~margin="1em", ());

let fullsizeStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~width="100%",
    ~display="flex",
    ~flexDirection="column",
    (),
  );

let headerTabStyle = ReactDOMRe.Style.make(~width="100%", ());

let titleStyle =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="flex-start", ());

let tabsStyle =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="flex-end", ());

let classNameTextStyle =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~marginBottom="1em",
    ~borderBottom="solid 1px gray",
    (),
  );

let rowStyle = ReactDOMRe.Style.make(~width="100%", ~margin="0", ());

let tabTypeToComponent =
    (resultTabType, classroom, testIds, normalized, updateNormalizr) =>
  switch (resultTabType) {
  | "roster" => <RosterLayout classroom testIds normalized updateNormalizr />
  | "takeTest" => <TakeTest testIds classroom normalized updateNormalizr />
  | "results" =>
    <ClassroomTabbedSpreadsheets classroom normalized updateNormalizr />
  | _ => <div />
  };

let make =
    (
      ~classroomId,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~testIds,
      _children,
    ) => {
  ...component,
  render: _self =>
    switch (
      MyNormalizr.Converter.Classroom.Remote.getRecord(
        normalized,
        classroomId,
      )
    ) {
    | None => <div />
    | Some(classroom) =>
      let updateClassroom = action =>
        MyNormalizr.Converter.Classroom.Remote.updateWithDefault(
          (),
          normalized |> Js.Promise.resolve,
          Schema.Classroom.stringToId(classroom.data.id),
          action,
        )
        |> updateNormalizr;

      let noStudents = Belt.List.length(classroom.data.studentIds) == 0;
      let noTests = Belt.List.length(testIds) == 0;
      let noClasroomTests = Belt.List.length(classroom.data.testIds) == 0;
      let noStudentsOrNoClassroomTests = noStudents || noClasroomTests;
      let noStudentsOrNoTests = noStudents || noTests;

      <div style=fullsizeStyle>
        <ContentHeader>
          <Row style=rowStyle>
            <Col md=6>
              <div style=titleStyle>
                <h1> {ReasonReact.string(classroom.data.name)} </h1>
              </div>
            </Col>
            <Col md=6>
              <div style=tabsStyle>
                <TabNav className="ClassroomLayout__Tabs">
                  <TabNavItem className="ClassroomLayout__TabItem">
                    <TabNavLink
                      className={
                        "ClassroomLayout__TabLink"
                        ++ (classroom.local.tab == "roster" ? " active" : "")
                      }
                      onClick=(
                        () => updateClassroom(ChangeClassroomTab("roster"))
                      )>
                      {ReasonReact.string("Roster")}
                    </TabNavLink>
                  </TabNavItem>
                  {
                    noStudentsOrNoTests ?
                      <div /> :
                      <TabNavItem className="ClassroomLayout__TabItem">
                        <TabNavLink
                          className={
                            "ClassroomLayout__TabLink"
                            ++ (
                              classroom.local.tab == "takeTest" ?
                                " active" : ""
                            )
                          }
                          onClick=(
                            () =>
                              updateClassroom(ChangeClassroomTab("takeTest"))
                          )>
                          {ReasonReact.string("Grade Test")}
                        </TabNavLink>
                      </TabNavItem>
                  }
                  {
                    noStudentsOrNoClassroomTests ?
                      <div /> :
                      <TabNavItem className="ClassroomLayout__TabItem">
                        <TabNavLink
                          className={
                            "ClassroomLayout__TabLink"
                            ++ (
                              classroom.local.tab == "results" ? " active" : ""
                            )
                          }
                          onClick=(
                            () =>
                              updateClassroom(ChangeClassroomTab("results"))
                          )>
                          {ReasonReact.string("Test Results")}
                        </TabNavLink>
                      </TabNavItem>
                  }
                </TabNav>
              </div>
            </Col>
          </Row>
        </ContentHeader>
        <Tabs activeTab={classroom.local.tab}>
          /* Using CSS class */

            <Tab tabId={classroom.local.tab}>
              {
                tabTypeToComponent(
                  classroom.local.tab,
                  classroom,
                  testIds,
                  normalized,
                  updateNormalizr,
                )
              }
            </Tab>
          </Tabs>
      </div>;
    },
};