module UpdateStudentAnswerKey = [%graphql
  {|
    mutation updateStudentAnswerKey($id: ID!, $correct: Boolean!) {
      updateStudentAnswerKey(studentAnswerKey: {id: $id, correct: $correct}){
        ...StudentAnswerKey_Model.Fragment.StudentAnswerKeyFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("UpdateStudentAnswerKey");

module UpdateStudentAnswerKeyMutation =
  ReasonApollo.CreateMutation(UpdateStudentAnswerKey);

let make = children => {
  ...component,
  render: _ =>
    <UpdateStudentAnswerKeyMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~correct) => {
               let updateStudentAnswerKey =
                 UpdateStudentAnswerKey.make(~id, ~correct, ());
               mutation(
                 ~variables=updateStudentAnswerKey##variables,
                 ~refetchQueries=[|"member"|],
                 (),
               );
             })
         }
    </UpdateStudentAnswerKeyMutation>,
};