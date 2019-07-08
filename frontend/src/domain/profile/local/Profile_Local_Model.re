type teacherModal =
  | Classroom
  | Test;

type _record = {
  subSidebarSelection: string,
};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {
    subSidebarSelection: "",
  };
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};