let getStudentAnswerKeys =
    (
      normalized,
      question: Question.Model.Record.t,
      studentQuestion: StudentQuestion.Model.Record.t,
    ) => {
  let questionBaseId = Question_Utils.questionToBaseId(question);

  let questionBase =
    normalized
    |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
    |> Belt.Option.getWithDefault(
         _,
         QuestionBase.Model.Record.defaultWithId((), questionBaseId),
       );

  questionBase.data.answerKeyIds
  |> Belt.List.map(_, answerKeyId =>
       studentQuestion.data.answerKeyIds
       |> MyNormalizr.Converter.StudentAnswerKey.idListToFilteredItems(
            _,
            MyNormalizr.Converter.StudentAnswerKey.Remote.getRecord(
              normalized,
            ),
          )
       |> Belt.List.getBy(_, studentAnswerKey => studentAnswerKey.data.originalId == answerKeyId)
       |> Belt.Option.getWithDefault(
            _, StudentAnswerKey.Model.Record.default(answerKeyId |> QuestionAnswerKey.Model.getUUIDFromId),
          )
     );
};

let createStudentQuestionAddKeyAnswer =
    (
      question: Question.Model.Record.t,
      studentQuestion: StudentQuestion.Model.Record.t,
      giveAnswerKeyToQuestionMutation,
      studentAnswerKey: StudentAnswerKey.Model.Record.t,
      norm,
    ) => {
  let typedId = Question_Utils.idToTypedId(question);

  let numAnswerKeysInList =
    studentQuestion.data.answerKeyIds
    |> Belt.List.keep(_, answerKeyId =>
         answerKeyId |> StudentAnswerKey.Model.getUUIDFromId == studentAnswerKey.data.id
       )
    |> Belt.List.length;

  let hasAnswerKeyInList = numAnswerKeysInList != 0;

  hasAnswerKeyInList ?
    norm :
    norm
    |> MyNormalizr.Converter.StudentQuestion.Remote.updateWithDefault(
         typedId,
         _,
         Schema.StudentQuestion.stringToId(studentQuestion.data.id),
         ApolloGiveAnswerKeyToQuestion(
           () =>
             giveAnswerKeyToQuestionMutation(
               ~id=studentAnswerKey.data.id,
               ~originalId=
                 studentAnswerKey.data.originalId |> QuestionAnswerKey.Model.getUUIDFromId,
               ~studentQuestionId=studentQuestion.data.id,
             ),
         ),
       );
};
let createUpdateStudentAnswer =
    (
      normalized,
      updateNormalizedWithReducer,
      updateNormalizr,
      question: Question.Model.Record.t,
      studentQuestion: StudentQuestion.Model.Record.t,
      studentAnswerKeys,
      studentQuestionAddKeyAnswer,
      updateStudentQuestionMutation,
      updateStudentAnswerKey,
    ) => {
  let questionId = Question_Utils.idToTypedId(question);

  let reduceNormalizedAllAnswerKeys = (memoNormalized, action) =>
    Belt.List.reduce(
      studentAnswerKeys,
      memoNormalized,
      (memoNormalized, studentAnswerKey: StudentAnswerKey.Model.Record.t) =>
      memoNormalized
      |> studentQuestionAddKeyAnswer(studentAnswerKey)
      |> MyNormalizr.Converter.StudentAnswerKey.Remote.updateWithDefault(
           studentAnswerKey.data.originalId |> QuestionAnswerKey.Model.getUUIDFromId,
           _,
           Schema.StudentAnswerKey.stringToId(studentAnswerKey.data.id),
           action,
         )
    );

  /* this is kind of confusing */
  let normalizedAllStudentKeys = (memoNormalized, correct) =>
    reduceNormalizedAllAnswerKeys(
      memoNormalized,
      StudentAnswerKey.Action.ApollSetCorrect(
        updateStudentAnswerKey,
        correct,
      ),
    ); /* thus feels weird */

  let normaliedStudentQuestion = (norm, action) =>
    MyNormalizr.Converter.StudentQuestion.Remote.updateWithDefault(
      questionId,
      norm,
      Schema.StudentQuestion.stringToId(studentQuestion.data.id),
      action,
    );

  answer =>
    normalized
    |> Js.Promise.resolve
    |> updateNormalizedWithReducer
    |> normaliedStudentQuestion(
         _,
         StudentQuestion.Action.ApolloUpdateStudentQuestion(
           () =>
             updateStudentQuestionMutation(
               ~id=studentQuestion.data.id,
               ~answer,
             ),
         ),
       )
    |> (
      norm => {
        let questionBaseId = Question_Utils.questionToBaseId(question);
        let questionBase =
          normalized
          |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(
               _, questionBaseId)
          |> Belt.Option.getWithDefault(
               _, QuestionBase.Model.Record.defaultWithId((), questionBaseId));
        (
          questionBase.data.autoScore ?
            /* this is kind of confusing YUCK */
            normalizedAllStudentKeys(
              norm,
              answer == questionBase.data.solution,
            ) :
            norm
        )
        |> updateNormalizr;
      }
    );
};