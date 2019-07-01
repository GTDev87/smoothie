type _data = {
  id: UUID.t,
  min: float,
  max: float,
  totalScore: int,
  testName: string,
  reliability: float,
  numStudents: int,
  mean: float,
  median: float,
  standardDeviation: float,
  testId: Schema.Test.id,
  studentTestAnalyticsIds: list(Schema.StudentTestAnalytics.id),
  startedStudentIds: list(Schema.Student.id),
  unstartedStudentIds: list(Schema.Student.id),
  answerKeyAnalyticsIds: list(Schema.AnswerKeyAnalytics.id),
  objectiveAnalyticsIds: list(Schema.ObjectiveAnalytics.id),
  /* UI */
};

type _local = unit;

type _record = RecordType.t(_data, _local);

let _defaultData = (id : UUID.t): _data => {
  {
    id: id,
    min: 0.0,
    max: 0.0,
    mean: 0.0,
    median: 0.0,
    numStudents: 0,
    testName: "",
    standardDeviation: 0.0,
    totalScore: 0,
    reliability: 0.0,
    testId: Schema.Test.stringToId(id),
    studentTestAnalyticsIds: [],
    startedStudentIds: [],
    unstartedStudentIds: [],
    answerKeyAnalyticsIds: [],
    objectiveAnalyticsIds: [],
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