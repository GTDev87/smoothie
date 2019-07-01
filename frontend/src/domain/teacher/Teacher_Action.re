/* done */
type action =
  | ApolloAddClassroom(ApolloType.t)
  | ApolloAddTest(ApolloType.t)
  | SelectSideBar(SideTab.t, string)
  | ToggleDropdown(SideTab.t)
  | OpenModal(option(Teacher_Local_Model.teacherModal))
  | SelectClassroomId(option(Schema.Classroom.id))
  | CombineReducer(action, action)
  | NoOpKeyDown;

type model = Teacher_Model.Record.t;

let rec reduce = (action, promise: Js.Promise.t(model)): Js.Promise.t(model) =>
  promise
  |> Js.Promise.then_((teacher: model) => {
       let {local, data: {classroomIds, testIds}}: model = teacher;
       let {newClassroomId, newTestId}: Teacher_Local.Model.Record.t = local;

       switch (action) {
       /* both below */
       | ApolloAddClassroom(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(
                {
                  local:
                    Teacher_Local.Action.reduce(
                      Teacher_Local.Action.ResetNewClassroom,
                      local,
                    ),
                  data: teacher.data,
                }: model,
              ),
            )
       | ApolloAddTest(apollo) =>
         apollo
         |> ApolloType.runApollo
         |> Utils.Promise.runBothIgnoreFirst(
              _,
              Js.Promise.resolve(
                {
                  local:
                    Teacher_Local.Action.reduce(
                      Teacher_Local.Action.ResetNewTest,
                      local,
                    ),
                  data: teacher.data,
                }: model,
              ),
            )
       /* mutation below */
       | CombineReducer(action1, action2) =>
         ActionUtil.combineActions(reduce, teacher, action1, action2)

       /* local below */
       | SelectSideBar(sidebar, subSidebar) =>
         {
           ...teacher,
           local:
             Teacher_Local.Action.reduce(
               Teacher_Local.Action.SelectSideBar(sidebar, subSidebar),
               local,
             ),
         }
         |> Js.Promise.resolve
       | ToggleDropdown(sidebar) =>
         {
           ...teacher,
           local:
             Teacher_Local.Action.reduce(
               Teacher_Local.Action.ToggleDropdown(sidebar),
               local,
             ),
         }
         |> Js.Promise.resolve
       | OpenModal(sidebar) =>
         {
           ...teacher,
           local:
             Teacher_Local.Action.reduce(
               Teacher_Local.Action.OpenModal(sidebar),
               local,
             ),
         }
         |> Js.Promise.resolve
       | SelectClassroomId(id) =>
         {
           ...teacher,
           local:
             Teacher_Local.Action.reduce(
               Teacher_Local.Action.SelectClassroomId(id),
               local,
             ),
         }
         |> Js.Promise.resolve
       | NoOpKeyDown => teacher |> Js.Promise.resolve
       };
     });