type action =
  | ApolloUpdateStimulus(ApolloType.t)
  | ToggleEditText
  | NoOp;

type model = Stimulus_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(stimulus => {
       let {local}: model = stimulus;

       switch (action) {
       | ApolloUpdateStimulus(apollo) =>
         apollo()
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(stimulus))
       | ToggleEditText =>
         {
           ...stimulus,
           local:
             Stimulus_Local.Action.reduce(
               Stimulus_Local.Action.ToggleEditText,
               local,
             ),
         }
         |> Js.Promise.resolve
       | NoOp => stimulus |> Js.Promise.resolve
       };
     });