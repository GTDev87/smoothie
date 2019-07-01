type _data = {
  id: UUID.t,
  /* UI */
  name: string,
  score: float,
  percentage: float,
  studentTestId: Schema.StudentTest.id,
  studentAnswerKeyIds: list(Schema.StudentAnswerKey.id)
};

type _local = unit;

type _record = RecordType.t(_data, _local);

let _defaultData = (id : UUID.t): _data => {
  {
    id: id,
    name: "",
    score: 0.,
    percentage: 0.,
    studentTestId: Schema.StudentTest.stringToId(id),
    studentAnswerKeyIds: [],
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