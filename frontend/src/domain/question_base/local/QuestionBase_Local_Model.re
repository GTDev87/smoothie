type _record = {
  editingText: bool,
  editingSolution: bool,
  newAnswerKeyId: Schema.QuestionAnswerKey.id,
};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);
  {
    editingText: true,
    editingSolution: false,
    newAnswerKeyId: Schema.QuestionAnswerKey.stringToId(Rand.generateSeqUUID()),
  };
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};