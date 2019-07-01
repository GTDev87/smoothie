type _record = {editingScore: bool};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {editingScore: false};
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};