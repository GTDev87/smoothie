type _record = {newOption: Choice.t};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {
    newOption: {
      id: Rand.generateSeqUUID(),
      text: "",
    },
  };
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};