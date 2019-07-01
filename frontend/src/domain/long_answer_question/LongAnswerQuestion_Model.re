type _data = {
  id: UUID.t,
  questionBaseId: Schema.QuestionBase.id,
  /* UI */
};


module ModelSchema = Schema.LongAnswerQuestion;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = LongAnswerQuestion_Fragment;
let fragmentType = Fragment.fragmentType;

let fragmentName = Fragment.Fields.name;


type _local = unit;

type _record = RecordType.t(_data, _local);


let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = id => {id, questionBaseId: Schema.QuestionBase.stringToId(id)};

let _defaultRecord = id: _record => {
  data: _defaultData(id |> getUUIDFromId),
  local: (),
};

module Record = {
  type t = _record;
  type defaultParam = unit;
  type defaultFn = (defaultParam, idType) => t;
  let findId = (record : _record) => record.data.id;
  module Data = {
    type t = _data;
    let fromObject = (obj: Fragment.Fields.t): t => {
      id: obj##id,
      questionBaseId: QuestionBase.Model.objectToId(obj##questionBase),
    };
  };

  let default = id => _defaultRecord(id);
  let defaultWithId = ((), id: idType): _record => {
    ..._defaultRecord(id),
    data: {
      ..._defaultData(getUUIDFromId(id)),
      id: getUUIDFromId(id),
    },
  };

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: (),
  };
};


