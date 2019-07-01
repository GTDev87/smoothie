module AddClassroom = [%graphql
  {|
    mutation addClassroom($id: ID!, $name: String!, $teacherId: ID!) {
      addClassroom(classroom: {id: $id, name: $name, teacherId: $teacherId}){
        ...Teacher_Model.Fragment.TeacherFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddClassroom");

module AddClassroomMutation = ReasonApollo.CreateMutation(AddClassroom);

let make = (~id, ~teacherId, ~name, children) => {
  ...component,
  render: _ =>
    <AddClassroomMutation>
      ...{
           (mutation, _ /* Result of your mutation */) => {
             let newClassroom = AddClassroom.make(~id, ~name, ~teacherId, ());
             children(~addClassroom=() =>
               mutation(
                 ~variables=newClassroom##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               )
             );
           }
         }
    </AddClassroomMutation>,
};