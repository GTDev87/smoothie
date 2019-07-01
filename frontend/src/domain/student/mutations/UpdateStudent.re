module UpdateStudent = [%graphql
  {|
    mutation updateStudent($id: ID!, $firstName: String!, $lastName: String!, $gradeId: ID!) {
      updateStudent(student: {id: $id, firstName: $firstName, lastName: $lastName, gradeId: $gradeId}){
        ...Student_Model.Fragment.StudentFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("UpdateStudent");

module UpdateStudentMutation = ReasonApollo.CreateMutation(UpdateStudent);

let make = (~id, children) => {
  ...component,
  render: _ =>
    <UpdateStudentMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~apolloUpdateStudent=(~firstName, ~lastName, ~gradeId) => {
               let updateStudent =
                 UpdateStudent.make(~id, ~firstName, ~lastName, ~gradeId, ());
               mutation(
                 ~variables=updateStudent##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </UpdateStudentMutation>,
};