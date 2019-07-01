type _data = {
  id: UUID.t,
  choices: list(Choice.t),
};

type _local = MultipleChoice_Local.Model.Record.t;
type _record = RecordType.t(_data, _local);


module ModelSchema = Schema.MultipleChoice;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);


module Fragment = MultipleChoice_Fragment;
let fragmentType = Fragment.fragmentType;
let fragmentName = Fragment.Fields.name;

let _defaultData = (id: UUID.t): _data => {id, choices: []};



let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultRecordId = id: _record => {
  data: _defaultData(id),
  local: MultipleChoice_Local.Model.Record.default(id),
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
      choices:
        obj##choices
        |> Belt.List.fromArray
        |> Belt.List.map(_, obj => ({id: obj##id, text: obj##text}: Choice.t)) /* How do i get these? */,
    };
  };

  let default = _defaultRecord;
  let defaultWithId = ((), id) =>
    _defaultRecordId(id |> getUUIDFromId);

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: MultipleChoice_Local.Model.Record.default(obj##id),
  };
};


