type _record = {editingText: bool};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {editingText: true};
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};