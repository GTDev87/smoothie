let getStudentAnswerKeys = (normalized, student: Student.Model.Record.t, test: Test.Model.Record.t) => {
  student.data.testIds
  |> Belt.List.map(_, studentTestId =>
       normalized
       |> MyNormalizr.Converter.StudentTest.Remote.getRecord(_, studentTestId)
       |> Belt.Option.getWithDefault(_, StudentTest.Model.Record.defaultWithId(test.data.id, studentTestId)))
  |> Belt.List.getBy(_, (studentTest: StudentTest.Model.Record.t) => Test.Model.getUUIDFromId(studentTest.data.originalId) == test.data.id)
  |> Belt.Option.getWithDefault(_, StudentTest.Model.Record.default(test.data.id))
  |> (studentTest) =>
      Belt.List.map(test.data.questionIds, questionId => {
        Belt.List.map(studentTest.data.questionIds, (studentQuestionId) =>
          normalized
          |> MyNormalizr.Converter.StudentQuestion.Remote.getRecord(_, studentQuestionId)
          |> Belt.Option.getWithDefault(_, StudentQuestion.Model.Record.defaultWithId(LongAnswer(""), studentQuestionId)))
        |> Belt.List.getBy(_, (studentQuestion) => studentQuestion.data.originalId == questionId)
        |> Belt.Option.getWithDefault(_, StudentQuestion.Model.Record.default(LongAnswer("")))
        |> (studentQuestion) => {
          let question = Question.Model.Record.defaultWithId(questionId);
          let answerKeys = StudentQuestion_Function.getStudentAnswerKeys(normalized, question, studentQuestion)
          Belt.List.map(answerKeys, (answerKey) => answerKey.data.id |> StudentAnswerKey.Model.idToTypedId)
        }
      })
      |> Belt.List.flatten
      |> Belt.List.map(_, (studentAnswerKeyId) =>
          normalized
          |> MyNormalizr.Converter.StudentAnswerKey.Remote.getRecord(_, studentAnswerKeyId)
          |> Belt.Option.getWithDefault(_, StudentAnswerKey.Model.Record.defaultWithId("", studentAnswerKeyId)));
}

let getStudentScore = (normalized, student: Student.Model.Record.t, test: Test.Model.Record.t) => {
  let score = 
    normalized
    |> getStudentAnswerKeys(_, student, test)
    |> Belt.List.reduce(_, 0.0, (memo, studentAnswerKey) =>
        (
          memo
          +. (
            studentAnswerKey.data.correct ?
              normalized
              |> MyNormalizr.Converter.QuestionAnswerKey.Remote.getRecord(_, studentAnswerKey.data.originalId)
              |> Belt.Option.getWithDefault(
                  _, QuestionAnswerKey.Model.Record.defaultWithId((), studentAnswerKey.data.originalId))
              |> (answerKey => answerKey.data.score) :
            0.0
          )
        )
      );
    score
  };

let getStudentsScoreList = (normalized, students: list(Student.Model.Record.t), test: Test.Model.Record.t) =>
  Belt.List.map(students, (student: Student.Model.Record.t) =>
    (student, getStudentScore(normalized, student, test))
  );