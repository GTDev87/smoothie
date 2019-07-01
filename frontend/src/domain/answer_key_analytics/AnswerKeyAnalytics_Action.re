/* done */
type action =
  | NoOp;

type model = AnswerKeyAnalytics_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((teacher: model) => {
       switch (action) {
       | NoOp => teacher |> Js.Promise.resolve
       };
     });