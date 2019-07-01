let component = ReasonReact.statelessComponent("TestRawDownloadButton");

let css = Css.css;
let cx = Css.cx;
let tw = Css.tw;

let testDownloadButton = [%bs.raw {| css(tw`  `)|}];

let getQuestionAnswerKeyWithDefaultFromId = (normalized, id) => {
  normalized
  |> MyNormalizr.Converter.QuestionAnswerKey.Remote.getRecord(_, id)
  |> Belt.Option.getWithDefault(_, QuestionAnswerKey.Model.Record.defaultWithId((), id));
};

let getTestWithDefaultFromId = (normalized, id) => {
  normalized
    |> MyNormalizr.Converter.Test.Remote.getRecord(_, id)
    |> Belt.Option.getWithDefault(_, Test.Model.Record.defaultWithId((), id));
};


let questionIdToQuestionBase = (normalized, questionId) : QuestionBase.Model.Record.t => {
  let questionBaseId =
    normalized
    |> MyNormalizr.getQuestionFromSchema(_, questionId)
    |> Belt.Option.getWithDefault(_, Question.Model.Record.defaultWithId(questionId))
    |> Question_Utils.questionToBaseId;

  normalized
  |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
  |> Belt.Option.getWithDefault(_, QuestionBase.Model.Record.defaultWithId((), questionBaseId));
}
let getStudentAnswerKeysFromStudentQuestion = (
  normalized,
  questionAnswerKeyId : QuestionAnswerKey.Model.idType,
  studentQuestion : StudentQuestion.Model.Record.t
) : StudentAnswerKey.Model.Record.t => {
  let questionAnswerKey =
    normalized
    |> MyNormalizr.Converter.QuestionAnswerKey.Remote.getRecord(_, questionAnswerKeyId)
    |> Belt.Option.getWithDefault(_, QuestionAnswerKey.Model.Record.defaultWithId((), questionAnswerKeyId));

  studentQuestion.data.answerKeyIds
  |> Belt.List.map(_, studentAnswerKeyId =>
    normalized
    |> MyNormalizr.Converter.StudentAnswerKey.Remote.getRecord(_, studentAnswerKeyId)
    |> Belt.Option.getWithDefault(_, StudentAnswerKey.Model.Record.defaultWithId(questionAnswerKey.data.id, studentAnswerKeyId)))
  |> Belt.List.getBy(_, (studentAnswerKey: StudentAnswerKey.Model.Record.t) =>
      QuestionAnswerKey.Model.getUUIDFromId(studentAnswerKey.data.originalId) == questionAnswerKey.data.id)
  |> Belt.Option.getWithDefault(_, StudentAnswerKey.Model.Record.default(questionAnswerKey.data.id));
};

let getStudentQuestionFromStudentTest = (
  normalized,
  questionId : Question.Model.idType,
  studentTest : StudentTest.Model.Record.t
) : StudentQuestion.Model.Record.t => {
  let questionBase = questionIdToQuestionBase(normalized, questionId);

  
  studentTest.data.questionIds
  |> Belt.List.map(_, studentQuestionId =>
    normalized
    |> MyNormalizr.Converter.StudentQuestion.Remote.getRecord(_, studentQuestionId)
    |> Belt.Option.getWithDefault(_, StudentQuestion.Model.Record.defaultWithId(questionId, studentQuestionId)))
  |> Belt.List.getBy(_, (studentQuestion: StudentQuestion.Model.Record.t) =>
      (studentQuestion.data.originalId |> Schema.getQuestionUUIDFromId) == questionBase.data.id)
  |> Belt.Option.getWithDefault(_, StudentQuestion.Model.Record.default(questionId));
};

let getStudentTestFromStudent = (
  normalized,
  testId : Test.Model.idType,
  student : Student.Model.Record.t
) : StudentTest.Model.Record.t => {
  let test = getTestWithDefaultFromId(normalized, testId);

  
  student.data.testIds
  |> Belt.List.map(_, studentTestId =>
    normalized
    |> MyNormalizr.Converter.StudentTest.Remote.getRecord(_, studentTestId)
    |> Belt.Option.getWithDefault(_, StudentTest.Model.Record.defaultWithId(test.data.id, studentTestId)))
  |> Belt.List.getBy(_, (studentTest: StudentTest.Model.Record.t) =>
      Test.Model.getUUIDFromId(studentTest.data.originalId) == test.data.id)
  |> Belt.Option.getWithDefault(_, StudentTest.Model.Record.default(test.data.id));
};

let questionIdToQuestionBase = (normalized, questionId) => {
  let questionBaseId =
    normalized
    |> MyNormalizr.getQuestionFromSchema(_, questionId)
    |> Belt.Option.getWithDefault(_, Question.Model.Record.defaultWithId(questionId))
    |> Question_Utils.questionToBaseId;

  normalized
  |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
  |> Belt.Option.getWithDefault(_, QuestionBase.Model.Record.defaultWithId((), questionBaseId));
};

type dataTable = {
  student: Student.Model.Record.t,
  test: Test.Model.Record.t,
  studentTest: StudentTest.Model.Record.t,
  questionBase: QuestionBase.Model.Record.t,
  studentQuestion: StudentQuestion.Model.Record.t,
  questionWithStudentAnswerKeys: list((QuestionAnswerKey.Model.Record.t, StudentAnswerKey.Model.Record.t)),
};

let getListDataTables = (normalized, test : Test.Model.Record.t, students) :list((Student.Model.Record.t, list(dataTable))) => {
  Belt.List.map(students, (student) => {
    let studentTest =
      getStudentTestFromStudent(normalized, Schema.Test.stringToId(test.data.id), student);

    (
      student,
      Belt.List.map(test.data.questionIds, (questionId) => {
        let studentQuestion =
          getStudentQuestionFromStudentTest(normalized, questionId, studentTest);
        
        let questionBase = questionIdToQuestionBase(normalized, questionId);
        
        let questionWithStudentAnswerKeys =
          Belt.List.map(questionBase.data.answerKeyIds, (answerKeyId) => {
            let questionAnswerKey = getQuestionAnswerKeyWithDefaultFromId(normalized, answerKeyId);
            let studentAnswerKeysForQuestion = getStudentAnswerKeysFromStudentQuestion(normalized, answerKeyId, studentQuestion);
            (questionAnswerKey, studentAnswerKeysForQuestion)
          });

        {
          student: student,
          test: test,
          studentTest: studentTest,
          questionBase: questionBase,
          studentQuestion: studentQuestion,
          questionWithStudentAnswerKeys: questionWithStudentAnswerKeys,
        };

      })
    );
  });
};

let makeHeaderFromTest = (normalized, test: Test.Model.Record.t) : list(string) => {
  let questionHeaderListData =
    test.data.questionIds
    |> Belt.List.mapWithIndex(_, (qIdx, questionId) => {
        let questionBase = questionIdToQuestionBase(normalized, questionId);

        ["Q." ++ string_of_int(qIdx + 1) ++ "_text"]
        /* @ (
            questionBase.data.answerKeyIds
            |> Belt.List.mapWithIndex(_, (akIdx, answerKeyId) => {
              "Q." ++ string_of_int(qIdx + 1) ++ "_" ++ string_of_int(akIdx)
            })
        ) */
    })
    |> Belt.List.flatten;
  [""] @ questionHeaderListData;
};

let makeRowFromTableFromTest = ((student: Student.Model.Record.t, dataTableList: list(dataTable))) : list(string) => {
  [
    student.data.firstName ++ " " ++ student.data.lastName,
    
  ] @ 
  (
    dataTableList
    |> Belt.List.map(_, (dataTable: dataTable) => {
      [
        dataTable.studentQuestion.data.answer
      ]
      /* @ (
          dataTable.questionWithStudentAnswerKeys
          |> Belt.List.map(_, ((questionAnswerKey, studentAnswerKey)) => {
            studentAnswerKey.data.correct ? "true" : "false"
          })
        ) */
    })
    |> Belt.List.flatten
  );
};

let formatData = (normalized, test : Test.Model.Record.t, students) : array(array(string)) => {
  (
    [
      makeHeaderFromTest(normalized, test) |> Belt.List.toArray
    ] @ 
    (
      normalized
      |> getListDataTables(_, test, students)
      |> Belt.List.map(_, (dt) => dt |> makeRowFromTableFromTest |> Belt.List.toArray)
    )
  )
  |> Belt.List.toArray;
};

let make = (
  ~test: Test.Model.Record.t,
  ~students,
  ~normalized,
  _children
) => {
  ...component,
  render: _self => {

    let data = formatData(normalized, test, students);
    <div className=testDownloadButton>
      <DownloadCSV data>
        <Button theme=CTA>
          {ReasonReact.string("Download Raw")}
        </Button>
      </DownloadCSV>
    </div>
  }
};