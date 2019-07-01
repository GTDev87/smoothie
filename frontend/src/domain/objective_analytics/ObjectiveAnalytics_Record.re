type _data = {
  id: UUID.t,
  objectiveName: string,
  percentageCorrect: float,
  questionIds: list(Schema.QuestionBase.id),
  masterStudentIds: list(Schema.Student.id),
  developingStudentIds: list(Schema.Student.id),
  /* UI */
};

type _local = unit;

type _record = RecordType.t(_data, _local);

let _defaultData = (id : UUID.t): _data => {
  {
    id: id,
    objectiveName: "",
    percentageCorrect: 0.0,
    questionIds: [],
    masterStudentIds: [],
    developingStudentIds: [],
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