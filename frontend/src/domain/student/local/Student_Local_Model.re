type _record = {
  edittingFirstName: bool,
  edittingLastName: bool,
};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {edittingFirstName: false, edittingLastName: false};
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};