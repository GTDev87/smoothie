type action =
  | ResetNewStudent
  | ChangeClassroomTab(string)
  | ChangeTestTab(Test.Model.idType)
  | SelectStudent(option(Schema.Student.id));

type model = Classroom_Local_Model.Record.t;

let reduce = (action, local: model) =>
  switch (action) {
  | ResetNewStudent => {
      ...local,
      newStudentId: Schema.Student.stringToId(UUID.generateUUID())
    }
  | ChangeClassroomTab(tab) => {...local, tab}
  | ChangeTestTab(testTab) => {...local, testTab: Some(testTab)}
  | SelectStudent(selectedStudent) => {...local, selectedStudent}
  };