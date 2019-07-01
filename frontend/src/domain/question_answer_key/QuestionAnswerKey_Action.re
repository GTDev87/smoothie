type action =
  | ApolloUpdateQuestionAnswer(ApolloType.t)
  | ToggleEditScore
  | NoOp;

type model = QuestionAnswerKey_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(answerKey => {
       let {local}: model = answerKey;

       switch (action) {
       /* mutation below */
       | ApolloUpdateQuestionAnswer(apollo) =>
         apollo()
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(answerKey),
            )

       /* local below */
       | ToggleEditScore =>
         {
           ...answerKey,
           local:
             QuestionAnswerKey_Local.Action.reduce(
               QuestionAnswerKey_Local.Action.ToggleEditScore,
               local,
             ),
         }
         |> Js.Promise.resolve
       | NoOp => answerKey |> Js.Promise.resolve
       };
     });