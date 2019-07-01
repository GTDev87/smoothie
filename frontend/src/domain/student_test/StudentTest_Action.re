type action =
  | ApolloGiveQuestionToTest(ApolloType.t)
  | NoOpKeyDown;

type model = StudentTest_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(studentTest =>
       switch (action) {
       /* mutation below */
       | ApolloGiveQuestionToTest(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(studentTest),
            )
       | NoOpKeyDown => studentTest |> Js.Promise.resolve
       }
     );