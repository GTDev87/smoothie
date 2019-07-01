type action =
  | ResetNewClassroom
  | ResetNewTest
  | SelectSideBar(SideTab.t, string)
  | SelectClassroomId(option(Schema.Classroom.id))
  | ToggleDropdown(SideTab.t)
  | OpenModal(option(Teacher_Local_Model.teacherModal));

type model = Teacher_Local_Model.Record.t;

let reduce = (action, local: model): model =>
  switch (action) {
  | ResetNewClassroom => {
      ...local,
      newClassroomId: Schema.Classroom.stringToId(UUID.generateUUID())
    }
  | ResetNewTest => {...local, newTestId: Schema.Test.stringToId(UUID.generateUUID())}
  | SelectSideBar(sidebarSelection, subSidebarSelection) => {
      ...local,
      sidebarSelection,
      subSidebarSelection,
    }
  | SelectClassroomId(selectedClassroomId) => {...local, selectedClassroomId}
  | ToggleDropdown(sidebarSelection) => {
      ...local,
      openDropdown:
        local.openDropdown
        |> Belt.List.map(_, ((dropdownType, isOpen)) =>
             sidebarSelection == dropdownType ?
               (sidebarSelection, !isOpen) : (dropdownType, isOpen)
           ),
    }
  | OpenModal(sidebarSelection) => {...local, openModal: sidebarSelection}
  };