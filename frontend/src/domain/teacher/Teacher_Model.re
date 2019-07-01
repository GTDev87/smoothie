type _data = {
  id: UUID.t,
  testIds: list(Schema.Test.id),
  classroomIds: list(Schema.Classroom.id),
  /* UI */
};

type _local = Teacher_Local.Model.Record.t;
type _record = RecordType.t(_data, _local);


module ModelSchema = Schema.Teacher;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = Teacher_Fragment;

let fragmentType = Fragment.fragmentType;
let fragmentName = Fragment.Fields.name;
let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = id => {
  id,
  testIds: [],
  classroomIds: [],
  /* UI */
};

let _defaultRecordId = id: _record => {
  data: _defaultData(id),
  local: Teacher_Local.Model.Record.default(id),
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
      testIds:
        obj##tests
        |> Belt.List.fromArray
        |> Belt.List.map(_, Test.Model.objectToId),
      classroomIds:
        obj##classrooms
        |> Belt.List.fromArray
        |> Belt.List.map(_, Classroom.Model.objectToId),
    };
  };

  let default = () => _defaultRecord();
  let defaultWithId = ((), id): t =>
    _defaultRecordId(id |> getUUIDFromId);

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: Teacher_Local.Model.Record.default(obj##id),
  };
};