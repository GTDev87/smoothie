let questionToBaseId = (question: Question_Model.Record.t) : QuestionBase.Model.idType =>
  switch (question) {
  | LongAnswerQuestion(question) => question.data.questionBaseId
  | MultipleChoiceQuestion(question) => question.data.questionBaseId
  | TrueFalseQuestion(question) => question.data.questionBaseId
  | FillInTheBlankQuestion(question) => question.data.questionBaseId
  };

/* Next funct belongs in model*/
let idToTypedId = (question) =>
  switch (question) {
  | Question.Model.LongAnswerQuestion(question) =>
    Schema.Question.LongAnswer(question.data.id)
  | Question.Model.MultipleChoiceQuestion(question) =>
    Schema.Question.MultipleChoice(question.data.id)
  | Question.Model.TrueFalseQuestion(question) =>
    Schema.Question.TrueFalse(question.data.id)
  | Question.Model.FillInTheBlankQuestion(question) =>
    Schema.Question.FillInTheBlank(question.data.id)
  };

/* Below are for debugging */
let questionToString = (question: Question.Model.Record.t): string =>
  switch (question) {
  | MultipleChoiceQuestion(_) => "Multiple Choice"
  | LongAnswerQuestion(_) => "Long Response"
  | TrueFalseQuestion(_) => "True False"
  | FillInTheBlankQuestion(_) => "Fill In The Blank"
  };

let questionIdToString = (normalized, questionId): string =>
  normalized
  |> MyNormalizr.getQuestionFromSchema(_, questionId)
  |> Belt.Option.getWithDefault(_, Question.Model.Record.defaultWithId(questionId))
  |> questionToString;

