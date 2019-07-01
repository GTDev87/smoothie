/* TODO not tested yet */
module GiveTestToStudent = [%graphql
  {|
    mutation giveTestToStudent($id: ID!, $studentId: ID!, $originalId: ID!) {
      giveTestToStudent(studentTest: {id: $id, studentId: $studentId, originalId: $originalId}){
        ...Student_Model.Fragment.StudentFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("GiveTestToStudent");

module GiveTestToStudentMutation =
  ReasonApollo.CreateMutation(GiveTestToStudent);

let make = children => {
  ...component,
  render: _ =>
    <GiveTestToStudentMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~giveTestToStudent=(~id, ~originalId, ~studentId) => {
               let giveTestToStudent =
                 GiveTestToStudent.make(~id, ~originalId, ~studentId, ());
               mutation(
                 ~variables=giveTestToStudent##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </GiveTestToStudentMutation>,
};