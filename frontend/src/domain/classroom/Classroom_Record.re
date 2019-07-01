type _data = {
  id: UUID.t,
  name: string,
  studentIds: list(Schema.Student.id),
  testIds: list(Schema.Test.id), /* need classroom test model */
  testAnalyticsIds: list(Schema.TestAnalytics.id) /* need classroom test model */
  /* UI */
};

type _local = Classroom_Local.Model.Record.t;

type _record = RecordType.t(_data, _local);

let _defaultData = (id) => {
  id,
  name: "",
  studentIds: [],
  testIds: [],
  testAnalyticsIds: [],
  /* UI */
};

let _defaultRecordId = (id): _record => {
  data: _defaultData(id),
  local: Classroom_Local.Model.Record.default(id),
};

let _defaultRecord = (): _record => {
  _defaultRecordId(UUID.generateUUID())
};

let findId = (record : _record) => record.data.id;