type action =
  | NoOp;

type model = FillInTheBlankQuestion_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_(longAnswerQuestion =>
       switch (action) {
       | NoOp => longAnswerQuestion |> Js.Promise.resolve
       }
     );