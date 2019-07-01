module CreateTeacher = [%graphql
  {|
  mutation createTeacher($id: ID!) {
    createTeacher(teacher: {id: $id}){
      ...User_Model.Fragment.UserFields
    }
  }
|}
];

let component = ReasonReact.statelessComponent("CreateTeacher");

module CreateTeacherMutation = ReasonApollo.CreateMutation(CreateTeacher);

let make = (~id, children) => {
  ...component,
  render: _ =>
    <CreateTeacherMutation>
      ...{
           (mutation, _ /* Result of your mutation */) => {
             let newTeacher = CreateTeacher.make(~id, ());
             children(~createTeacher=() =>
               mutation(
                 ~variables=newTeacher##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               )
             );
           }
         }
    </CreateTeacherMutation>,
};