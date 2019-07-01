type setCorrectMutation =
  (~id: UUID.t, ~correct: bool) => Js.Promise.t(ApolloType.renderPropObjJS);

type action =
  | ApolloStudentAnswerKey(ApolloType.t)
  | ApollSetCorrect(setCorrectMutation, bool)
  | NoOpKeyDown;

type model = StudentAnswerKey_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((studentStudentAnswer: model) =>
       switch (action) {
       /* mutation below */

       | ApollSetCorrect(mutationFn, correct) =>
         mutationFn(~id=studentStudentAnswer.data.id, ~correct)
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(studentStudentAnswer),
            )
       | ApolloStudentAnswerKey(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(studentStudentAnswer),
            )
       | NoOpKeyDown => Js.Promise.resolve(studentStudentAnswer)
       }
     );