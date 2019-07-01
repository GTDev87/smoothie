type classroomTab = string;

type _record = {
  newTestId: Schema.Test.id,
  newStudentId: Schema.Student.id,
  tab: classroomTab,
  testTab: option(Test.Model.idType),
  selectedStudent: option(Schema.Student.id),
};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);

  {
    newTestId: Schema.Test.stringToId(Rand.generateSeqUUID()),
    newStudentId: Schema.Student.stringToId(Rand.generateSeqUUID()),
    tab: "roster",
    testTab: None,
    selectedStudent: None,
  };
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};