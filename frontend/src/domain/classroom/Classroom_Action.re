type action =
  | AddStudent
  | ApolloAddStudent(ApolloType.t)
  | ApolloUpdateClassroom(ApolloType.t)
  | ApolloGiveTestToClassroom(ApolloType.t)
  | EditName(string)
  | ChangeClassroomTab(string)
  | ChangeTestTab(Test.Model.idType)
  | SelectStudent(option(Schema.Student.id))
  | CombineReducer(action, action)
  | NoOpKeyDown;

type model = Classroom_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((classroom: model) => {
       let {local, data: {studentIds, testIds}}: model = classroom;
       let {newStudentId}: Classroom_Local.Model.Record.t = local;
       switch (action) {
       /* both below */
       | AddStudent =>
         (
           {
             local:
               Classroom_Local.Action.reduce(
                 Classroom_Local.Action.ResetNewStudent,
                 local,
               ),
             data: {
               ...classroom.data,
               studentIds: studentIds @ [newStudentId],
             },
           }: model
         )
         |> Js.Promise.resolve

       | ApolloAddStudent(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(
                {
                  local:
                    Classroom_Local.Action.reduce(
                      Classroom_Local.Action.ResetNewStudent,
                      local,
                    ),
                  data: classroom.data,
                }: model,
              ),
            )
       | ApolloGiveTestToClassroom(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(classroom),
            )
       | ApolloUpdateClassroom(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(classroom),
            )
       /* mutation below */
       | EditName(name) =>
         {
           ...classroom,
           data: {
             ...classroom.data,
             name,
           },
         }
         |> Js.Promise.resolve
       | CombineReducer(action1, action2) =>
         ActionUtil.combineActions(reduce, classroom, action1, action2)

       /* local below */
       | ChangeClassroomTab(tab) =>
         {
           ...classroom,
           local:
             Classroom_Local.Action.reduce(
               Classroom_Local.Action.ChangeClassroomTab(tab),
               local,
             ),
         }
         |> Js.Promise.resolve
       | ChangeTestTab(tab) =>
         {
           ...classroom,
           local:
             Classroom_Local.Action.reduce(
               Classroom_Local.Action.ChangeTestTab(tab),
               local,
             ),
         }
         |> Js.Promise.resolve
       | SelectStudent(studentId) =>
         {
           ...classroom,
           local:
             Classroom_Local.Action.reduce(
               Classroom_Local.Action.SelectStudent(studentId),
               local,
             ),
         }
         |> Js.Promise.resolve

       | NoOpKeyDown => classroom |> Js.Promise.resolve
       };
     });