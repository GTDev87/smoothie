type _data = {
  id: UUID.t,
  score: float,
  objectiveId: option(Schema.Objective.id),
  /* UI */
};


module ModelSchema = Schema.QuestionAnswerKey;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = QuestionAnswerKey_Fragment;

let fragmentType = Fragment.fragmentType;
let fragmentName = Fragment.Fields.name;

type _local = QuestionAnswerKey_Local.Model.Record.t;
type _record = RecordType.t(_data, _local);

let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultDataId = id => {id, score: 1., objectiveId: None};

let _defaultRecordId = id: _record => {
  data: _defaultDataId(id),
  local: QuestionAnswerKey_Local.Model.Record.default(id),
};

let _defaultRecord = (): _record => _defaultRecordId(UUID.generateUUID());

module Record = {
  type t = _record;
  type defaultParam = unit;
  type defaultFn = (defaultParam, idType) => t;
  let findId = (record : _record) => record.data.id;
  module Data = {
    type t = _data;
    let fromObject = (obj: Fragment.Fields.t): t => {
      id: obj##id,
      score: obj##score,
      objectiveId:
        obj##objective |> Belt.Option.map(_, Objective.Model.objectToId),
    };
  };
  let default = () => _defaultRecord();
  let defaultWithId = ((), id) =>
    _defaultRecordId(id |> getUUIDFromId);

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: QuestionAnswerKey_Local.Model.Record.default(obj##id),
  };
};

