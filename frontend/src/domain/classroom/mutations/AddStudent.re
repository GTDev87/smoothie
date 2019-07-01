module AddStudent = [%graphql
  {|
    mutation addStudent($id: ID!, $classroomId: ID!) {
      addStudent(student: {id: $id, classroomId: $classroomId}){
        ...Classroom_Model.Fragment.ClassroomFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddStudent");

module AddStudentMutation = ReasonApollo.CreateMutation(AddStudent);

let make = (~id, ~classroomId, children) => {
  ...component,
  render: _ =>
    <AddStudentMutation>
      ...{
           (mutation, _ /* Result of your mutation */) => {
             let newStudent = AddStudent.make(~id, ~classroomId, ());
             children(~addStudent=() =>
               mutation(
                 ~variables=newStudent##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               )
             );
           }
         }
    </AddStudentMutation>,
};