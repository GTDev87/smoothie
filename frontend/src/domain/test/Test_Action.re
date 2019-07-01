type action =
  | ApolloAddObjective(ApolloType.t)
  | ApolloAddQuestion(ApolloType.t)
  | SelectResultTab(ResultTab.t)
  | EditName(string)
  | ApolloUpdateTest(ApolloType.t)
  | ApolloUpdateTestQuestion(ApolloType.t)
  | ToggleEditName
  | ToggleEditDescription
  | ToggleEditNotes
  | ChangeSelectedQuestion(option(UUID.t))
  | ChangeQuestionTabType(QuestionTab.t)
  | ChangeGraphDisplay(OverviewGraphDisplay.t)
  | CombineReducer(action, action)
  | NoOpKeyDown;

type model = Test_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((test: model) => {
       let {local}: model = test;

       switch (action) {
       /* both below */

       | ApolloAddQuestion(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve({
                ...test,
                local:
                  Test_Local.Action.reduce(
                    Test_Local.Action.ResetNewQuestion,
                    local,
                  ),
              }),
            )

       | ApolloUpdateTestQuestion(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(test))

       | ApolloAddObjective(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve({
                ...test,
                local:
                  Test_Local.Action.reduce(
                    Test_Local.Action.ResetNewObjective,
                    local,
                  ),
              }),
            )

       | EditName(name) =>
         {
           ...test,
           data: {
             ...test.data,
             name,
           },
         } |> Js.Promise.resolve

       /* mutation below */

       | ApolloUpdateTest(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(test))

       | CombineReducer(action1, action2) =>
         ActionUtil.combineActions(reduce, test, action1, action2)

       /* local below */
       | ToggleEditName =>
         {
           ...test,
           local:
             Test_Local.Action.reduce(
               Test_Local.Action.ToggleEditName,
               local,
             ),
         }
         |> Js.Promise.resolve
       | ToggleEditDescription =>
         {
           ...test,
           local:
             Test_Local.Action.reduce(
               Test_Local.Action.ToggleEditDescription,
               local,
             ),
         }
         |> Js.Promise.resolve
        | ToggleEditNotes =>
        {
          ...test,
          local:
            Test_Local.Action.reduce(
              Test_Local.Action.ToggleEditNotes,
              local,
            ),
        }
        |> Js.Promise.resolve
       | SelectResultTab(resultTab) =>
         {
           ...test,
           local:
             Test_Local.Action.reduce(
               Test_Local.Action.SelectResultTab(resultTab),
               local,
             ),
         }
         |> Js.Promise.resolve
       | ChangeQuestionTabType(questionTab) =>
         {
           ...test,
           local:
             Test_Local.Action.reduce(
               Test_Local.Action.ChangeQuestionTabType(questionTab),
               local,
             ),
         }
         |> Js.Promise.resolve
       | ChangeSelectedQuestion(selectedQuestion) =>
         {
           ...test,
           local:
             Test_Local.Action.reduce(
               Test_Local.Action.ChangeSelectedQuestion(selectedQuestion),
               local,
             ),
         }
         |> Js.Promise.resolve
       | ChangeGraphDisplay(overviewGraphDisplay) =>
         {
           ...test,
           local:
             Test_Local.Action.reduce(
               Test_Local.Action.ChangeGraphDisplay(overviewGraphDisplay),
               local,
             ),
         }
         |> Js.Promise.resolve
       | NoOpKeyDown => test |> Js.Promise.resolve
       };
     });