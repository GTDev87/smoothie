type _data = {
  id: UUID.t,
  text: string,
  solution: string,
  autoScore: bool,
  forcedResponse: bool,
  questionNumber: int,
  questionType: QuestionType.t,
  stimulusId: option(Schema.Stimulus.id),
  answerKeyIds: list(Schema.QuestionAnswerKey.id),
};

module ModelSchema = Schema.QuestionBase;
type idType = ModelSchema.id;
type rootIdType = ModelUtils.RootModel.id;
let idToRootId = ModelSchema.idToRootId;
let getUUIDFromId = ModelSchema.idToString;
let idToTypedId = (id: UUID.t): idType => ModelSchema.stringToId(id);

module Fragment = QuestionBase_Fragment;
let fragmentType = Fragment.fragmentType;

let fragmentName = Fragment.Fields.name;

/* Question Local */
type _local = QuestionBase_Local.Model.Record.t;
type _record = RecordType.t(_data, _local);


let objectToId = (obj: Fragment.Fields.t): idType => idToTypedId(obj##id);

let _defaultData = id => {
  module UUIDSeedType = {
    let namespace = id;
  };
  module Rand = UUID.V5Random(UUIDSeedType);

  {
    id,
    text: "",
    solution: "",
    autoScore: false,
    forcedResponse: false,
    stimulusId: None,
    questionType: `LONG_ANSWER,
    questionNumber: 0,
    answerKeyIds: [Schema.QuestionAnswerKey.stringToId(Rand.generateSeqUUID())] /* need to generate first answer key id */
  };
};

let _defaultRecordId = id: _record => {
  data: _defaultData(id),
  local: QuestionBase_Local.Model.Record.default(id),
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
      text: obj##text,
      solution: obj##solution,
      autoScore: obj##autoScore,
      forcedResponse: obj##forcedResponse,
      questionType: obj##questionType,
      questionNumber: obj##questionNumber,
      stimulusId:
        obj##stimulus |> Belt.Option.map(_, Stimulus.Model.objectToId),
      answerKeyIds:
        obj##answerKeys
        |> Belt.List.fromArray
        |> Belt.List.map(_, QuestionAnswerKey.Model.objectToId),
    };
  };

  let default = _defaultRecord;
  let defaultWithId = ((), id) =>
    _defaultRecordId(id |> getUUIDFromId);

  let fromObject = (obj: Fragment.Fields.t): t => {
    data: Data.fromObject(obj),
    local: QuestionBase_Local.Model.Record.default(obj##id),
  };
};


