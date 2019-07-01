type action =
  | ApolloGiveAnswerKeyToQuestion(ApolloType.t)
  | ApolloUpdateStudentQuestion(ApolloType.t)
  | NoOpKeyDown;

type model = StudentQuestion_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(studentQuestion =>
       switch (action) {
       /* mutation below */
       | ApolloGiveAnswerKeyToQuestion(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(studentQuestion),
            )
       | ApolloUpdateStudentQuestion(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(studentQuestion),
            )
       | NoOpKeyDown => studentQuestion |> Js.Promise.resolve
       }
     );