type _data = {
  id: UUID.t,
  questionIds: list(Schema.Question.id),
  objectiveIds: list(option(Schema.Objective.id)),
  notes: option(string),
  name: string,
  description: string,
  /* UI */
};

module ModelSchema = Schema.Test;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = Test_Fragment;

let fragmentType = Fragment.fragmentType;


let fragmentName = Fragment.Fields.name;

type _local = Test_Local.Model.Record.t;
type _record = RecordType.t(_data, _local);

let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = id => {
  id,
  name: "",
  notes: None,
  description: "",
  questionIds: [],
  objectiveIds: [None],
  /* UI */
};

let _defaultRecordId = id: _record => {
  data: _defaultData(id),
  local: Test_Local.Model.Record.default(id),
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
      name: obj##name,
      description: obj##description,
      notes: obj##notes,
      objectiveIds:
        obj##objectives
        |> Belt.List.fromArray
        |> Belt.List.map(_, obj => Some(Objective.Model.objectToId(obj))),
      questionIds:
        obj##questions
        |> Belt.List.fromArray
        |> Belt.List.map(_, obj => Question.Model.objectToId(obj)),
    };
  };

  let default = () => _defaultRecord();
  let defaultWithId = ((), id) =>
    _defaultRecordId(id |> getUUIDFromId);

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: Test_Local.Model.Record.default(obj##id),
  };
};

