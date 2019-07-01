type action =
  | ApolloEditText(ApolloType.t);

type model = Objective_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(objective =>
       switch (action) {
       /* mutation below */

       | ApolloEditText(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(objective),
            )
       }
     );