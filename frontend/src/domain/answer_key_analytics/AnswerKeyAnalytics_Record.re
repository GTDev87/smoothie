type histogramValue = {
  text: string,
  freq: int,
  correct: bool,
};

type _data = {
  id: UUID.t,
  /* UI */
  questionText: string,
  objectiveText: string,
  discrimination: float,
  studentQuestionAnswerIds: list(Schema.StudentAnswerKey.id),
  masterIds: list(Schema.Student.id),
  developingIds: list(Schema.Student.id),
  answerKeyId: Schema.QuestionAnswerKey.id,
  histogram: list(histogramValue),
};

type _local = unit;

type _record = RecordType.t(_data, _local);

let _defaultData = (id : UUID.t): _data => {
  {
    id: id,
    questionText: "",
    objectiveText: "",
    discrimination: 0.0,
    studentQuestionAnswerIds: [],
    masterIds: [],
    developingIds: [],
    answerKeyId: QuestionAnswerKey.Model.idToTypedId(""),
    histogram: [],
  }
};

let _defaultRecordId = (id): _record => {
  data: _defaultData(id),
  local: (),
};

let _defaultRecord = (): _record => {
  _defaultRecordId(UUID.generateUUID());
};

let findId = (record : _record) => record.data.id;