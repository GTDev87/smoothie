type action =
  | ApolloAddAnswerKey(ApolloType.t)
  | ApolloUpdateQuestion(ApolloType.t)
  | ApolloCreateStimulus(ApolloType.t)
  | ToggleEditText
  | ToggleEditSolution
  | CombineReducer(action, action)
  | NoOp;

type model = QuestionBase_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(question => {
       let {local}: model = question;

       switch (action) {
       /* both below */

       | ApolloAddAnswerKey(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(
                {
                  data: question.data,
                  local:
                    QuestionBase_Local.Action.reduce(
                      QuestionBase_Local.Action.ResetNewAnswerKey,
                      local,
                    ),
                }: model,
              ),
            )

       | ApolloUpdateQuestion(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(question))

       | ApolloCreateStimulus(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(question))

       | CombineReducer(action1, action2) =>
         ActionUtil.combineActions(reduce, question, action1, action2)

       /* local below */
       | NoOp => question |> Js.Promise.resolve
       | ToggleEditText =>
         {
           ...question,
           local: QuestionBase_Local.Action.reduce(ToggleEditText, local),
         }
         |> Js.Promise.resolve
       | ToggleEditSolution =>
         {
           ...question,
           local: QuestionBase_Local.Action.reduce(ToggleEditSolution, local),
         }
         |> Js.Promise.resolve
       };
     });