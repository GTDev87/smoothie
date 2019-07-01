type action =
  | EditNewOption(string)
  | ResetOption
  | NoOp;

type model = MultipleChoice_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(multipleChoice => {
       let {local}: model = multipleChoice;

       switch (action) {
       /* local below */
       | EditNewOption(newOption) =>
         {
           ...multipleChoice,
           local:
             MultipleChoice_Local.Action.reduce(
               EditNewOption(newOption),
               local,
             ),
         }
         |> Js.Promise.resolve
       | ResetOption =>
         {
           ...multipleChoice,
           local: MultipleChoice_Local.Action.reduce(ResetOption, local),
         }
         |> Js.Promise.resolve
       | NoOp => multipleChoice |> Js.Promise.resolve
       };
     });