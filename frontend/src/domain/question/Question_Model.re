/* not can't put the following into a union and pass around */

type _record =
  | LongAnswerQuestion(LongAnswerQuestion.Model.Record.t)
  | MultipleChoiceQuestion(MultipleChoiceQuestion.Model.Record.t)
  | TrueFalseQuestion(TrueFalseQuestion.Model.Record.t)
  | FillInTheBlankQuestion(FillInTheBlankQuestion.Model.Record.t);


type idType = Schema.Question.id;

type questionTypeObj = [
  | `LongAnswerQuestion(LongAnswerQuestion.Model.Fragment.Fields.t)
  | `MultipleChoiceQuestion(MultipleChoiceQuestion.Model.Fragment.Fields.t)
  | `TrueFalseQuestion(TrueFalseQuestion.Model.Fragment.Fields.t)
  | `FillInTheBlankQuestion(FillInTheBlankQuestion.Model.Fragment.Fields.t)
]
module Fragment = {
  module QuestionFields = {
    type t = questionTypeObj;
  };
};
/* let idToTypedId = (id: UUID.t): idType => id; */
let objectToId = (obj: questionTypeObj): idType =>
  switch (obj) {
  | `LongAnswerQuestion(q) =>
      Schema.Question.LongAnswer(LongAnswerQuestion.Model.objectToId(q) |> Schema.LongAnswerQuestion.idToString)
  | `MultipleChoiceQuestion(q) =>
      Schema.Question.MultipleChoice(MultipleChoiceQuestion.Model.objectToId(q) |> Schema.MultipleChoiceQuestion.idToString)
  | `TrueFalseQuestion(q) =>
      Schema.Question.TrueFalse(TrueFalseQuestion.Model.objectToId(q) |> Schema.TrueFalseQuestion.idToString)
  | `FillInTheBlankQuestion(q) =>
      Schema.Question.FillInTheBlank(FillInTheBlankQuestion.Model.objectToId(q) |> Schema.FillInTheBlankQuestion.idToString)
  };

module Record = {
  type t = _record;
  let defaultWithId = (id: idType): _record =>
    switch (id) {
    | LongAnswer(id) =>
      LongAnswerQuestion(
        LongAnswerQuestion.Model.Record.defaultWithId((), Schema.LongAnswerQuestion.stringToId(id)),
      )
    | MultipleChoice(id) =>
      MultipleChoiceQuestion(
        MultipleChoiceQuestion.Model.Record.defaultWithId((), Schema.MultipleChoiceQuestion.stringToId(id)),
      )
    | TrueFalse(id) =>
      TrueFalseQuestion(
        TrueFalseQuestion.Model.Record.defaultWithId((), Schema.TrueFalseQuestion.stringToId(id)),
      )
    | FillInTheBlank(id) =>
      FillInTheBlankQuestion(
        FillInTheBlankQuestion.Model.Record.defaultWithId((), Schema.FillInTheBlankQuestion.stringToId(id)),
      )
    };
};

let getById = (id: idType): option(questionTypeObj) =>
  switch (id) {
  | LongAnswer(laqId) =>
      LongAnswerQuestion.Container.getById(Schema.LongAnswerQuestion.stringToId(laqId))
      |> Belt.Option.map(_, obj => `LongAnswerQuestion(obj))
  | MultipleChoice(mcId) =>
      MultipleChoiceQuestion.Container.getById(Schema.MultipleChoiceQuestion.stringToId(mcId))
      |> Belt.Option.map(_, obj => `MultipleChoiceQuestion(obj))
  | TrueFalse(mcId) =>
      TrueFalseQuestion.Container.getById(Schema.TrueFalseQuestion.stringToId(mcId))
      |> Belt.Option.map(_, obj => `TrueFalseQuestion(obj))
  | FillInTheBlank(mcId) =>
      FillInTheBlankQuestion.Container.getById(Schema.FillInTheBlankQuestion.stringToId(mcId))
      |> Belt.Option.map(_, obj => `FillInTheBlankQuestion(obj))
  };
