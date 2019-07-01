let totalScore = (normalized, test: Test.Model.Record.t) =>
  test.data.questionIds
  |> Belt.List.map(_, questionId =>
       normalized
       |> MyNormalizr.getQuestionFromSchema(_, questionId)
       |> Belt.Option.getWithDefault(
            _,
            Question.Model.Record.defaultWithId(questionId),
          )
     )
  |> Belt.List.reduce(
       _,
       0.0,
       (memoQuestionVal, question: Question.Model.Record.t) => {
        let questionBaseId = Question_Utils.questionToBaseId(question);
         let questionBase =
           normalized
           |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(
                _,
                questionBaseId,
              )
           |> Belt.Option.getWithDefault(
                _,
                QuestionBase.Model.Record.defaultWithId((), questionBaseId),
              );

         questionBase.data.answerKeyIds
         |> Belt.List.map(_, answerKeyId =>
              normalized
              |> MyNormalizr.Converter.QuestionAnswerKey.Remote.getRecord(
                   _,
                   answerKeyId,
                 )
              |> Belt.Option.getWithDefault(
                   _,
                   QuestionAnswerKey.Model.Record.defaultWithId(
                     (),
                     answerKeyId,
                   ),
                 )
            )
         |> Belt.List.reduce(_, memoQuestionVal, (memo, answerKey) =>
              memo +. answerKey.data.score
            );
       },
     );