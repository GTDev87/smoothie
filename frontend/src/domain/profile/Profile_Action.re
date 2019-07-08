/* done */
type action =
  | NoOpKeyDown;

type model = Profile_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((profile: model) => {
       switch (action) {
       /* both below */
       | NoOpKeyDown => profile |> Js.Promise.resolve
       };
     });