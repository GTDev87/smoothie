type action =
  | ApolloGiveTestToStudent(ApolloType.t)
  | ApolloUpdateStudent(ApolloType.t)
  | NoOpKeyDown
  | ToggleEditFirstName
  | ToggleEditLastName
  | NoOp;

type model = Student_Model.Record.t;

let reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((student: model) => {
       let {data: {testIds}, local}: model = student;
       switch (action) {
       /* mutation below */

       | ApolloGiveTestToStudent(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(student))

       | ApolloUpdateStudent(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(_, Js.Promise.resolve(student))

       /* local below */
       | NoOpKeyDown => student |> Js.Promise.resolve
       | ToggleEditFirstName =>
         {
           ...student,
           local:
             Student_Local.Action.reduce(
               Student_Local.Action.ToggleEditFirstName,
               local,
             ),
         }
         |> Js.Promise.resolve
       | ToggleEditLastName =>
         {
           ...student,
           local:
             Student_Local.Action.reduce(
               Student_Local.Action.ToggleEditLastName,
               local,
             ),
         }
         |> Js.Promise.resolve
       | NoOp => student |> Js.Promise.resolve
       };
     });