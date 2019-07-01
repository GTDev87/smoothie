type action =
  | ApolloAddChoice(ApolloType.t);

type model = MultipleChoiceQuestion_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(multipleChoiceQuestion =>
       switch (action) {
       | ApolloAddChoice(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(multipleChoiceQuestion),
            )
       }
     );