type teacherModal =
  | Classroom
  | Test;

type _record = {
  newTestId: Schema.Test.id,
  newClassroomId: Schema.Classroom.id,
  addingNewClassroom: bool,
  sidebarSelection: SideTab.t,
  subSidebarSelection: string,
  selectedClassroomId: option(Schema.Classroom.id),
  openDropdown: list((SideTab.t, bool)),
  openModal: option(teacherModal),
};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {
    newTestId: Schema.Test.stringToId(Rand.generateSeqUUID()),
    newClassroomId: Schema.Classroom.stringToId(Rand.generateSeqUUID()),
    addingNewClassroom: false,
    sidebarSelection: SideTab.Dashboard,
    subSidebarSelection: "",
    selectedClassroomId: None,
    openDropdown: [(SideTab.Classroom, true), (SideTab.Test, true)],
    openModal: None,
  };
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};