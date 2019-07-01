type _data = {
  id: string,
  answer: string,
  originalId: Schema.Question.id,
  answerKeyIds: list(Schema.StudentAnswerKey.id),
  /* UI */
};

module ModelSchema = Schema.StudentQuestion;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);


module Fragment = StudentQuestion_Fragment;
let fragmentType = Fragment.fragmentType;

let fragmentName = Fragment.Fields.name;

type _local = unit;
type _record = RecordType.t(_data, _local);

let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = (questionId: Schema.Question.id) => {
  id: UUID.generateUUID(),
  answer: "",
  originalId: questionId,
  answerKeyIds: [],
};

let _defaultRecord =
    (questionId: Schema.Question.id): _record => {
  data: _defaultData(questionId),
  local: (),
};

module Record = {
  type t = _record;
  type defaultParam = Schema.Question.id;
  type defaultFn = (defaultParam, idType) => t;
  let findId = (record : _record) => record.data.id;
  module Data = {
    type t = _data;
    let fromObject = (obj: Fragment.Fields.t): t => {
      id: obj##id,
      answer: obj##answer,
      originalId: Question.Model.objectToId(obj##original),
      answerKeyIds:
        obj##answerKeys
        |> Belt.List.fromArray
        |> Belt.List.map(_, StudentAnswerKey.Model.objectToId),
    };
  };
  let default = question => _defaultRecord(question);
  let defaultWithId = (question, id): _record => {
    data: {
      ..._defaultData(question),
      id: getUUIDFromId(id),
    },
    local: (),
  };
  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: (),
  };
};


