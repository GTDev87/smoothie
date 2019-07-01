type _data = {
  id: string,
  questionIds: list(Schema.StudentQuestion.id),
  originalId: Schema.Test.id,
};

module ModelSchema = Schema.StudentTest;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = StudentTest_Fragment;
let fragmentType = Fragment.fragmentType;

let fragmentName = Fragment.Fields.name;

type _local = unit;
type _record = RecordType.t(_data, _local);

let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = (testId: UUID.t) => {
  id: UUID.generateUUID(),
  questionIds: [],
  originalId: Schema.Test.stringToId(testId),
};

let _defaultRecord = testId: _record => {
  data: _defaultData(testId),
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
      questionIds:
        obj##questions
        |> Belt.List.fromArray
        |> Belt.List.map(_, StudentQuestion.Model.objectToId),
      originalId: Test.Model.objectToId(obj##original),
    };
  };

  let default = _defaultRecord;
  let defaultWithId = (test, id): t => {
    data: {
      ..._defaultData(test),
      id: getUUIDFromId(id),
    },
    local: (),
  };

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: (),
  };
};

