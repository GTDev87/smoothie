type _data = {
  id: string,
  correct: bool,
  originalId: Schema.QuestionAnswerKey.id,
};

module ModelSchema = Schema.StudentAnswerKey;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = StudentAnswerKey_Fragment;
let fragmentType = Fragment.fragmentType;

let fragmentName = Fragment.Fields.name;

type _local = unit;
type _record = RecordType.t(_data, _local);

let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = (answerKeyId: UUID.t) => {
  id: UUID.generateUUID(),
  correct: false,
  originalId: Schema.QuestionAnswerKey.stringToId(answerKeyId),
};

let _defaultRecord = (answerKeyId: UUID.t): _record => {
  data: _defaultData(answerKeyId),
  local: (),
};

module Record = {
  type t = _record;
  type defaultParam = UUID.t;
  type defaultFn = (defaultParam, idType) => t;
  let findId = (record : _record) => record.data.id;

  module Data = {
    type t = _data;
    let fromObject = (obj: Fragment.Fields.t): t => {
      id: obj##id,
      correct: obj##correct,
      originalId: QuestionAnswerKey.Model.objectToId(obj##original),
    };
  };

  let default = answerKey => _defaultRecord(answerKey);
  let defaultWithId = (answerKey, id): _record => {
    data: {
      ..._defaultData(answerKey),
      id: getUUIDFromId(id),
    },
    local: (),
  };

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: (),
  };
};
