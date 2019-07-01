module UpdateStudentQuestion = [%graphql
  {|
    mutation updateStudentQuestion($id: ID!, $answer: String!) {
      updateStudentQuestion(studentQuestion: {id: $id, answer: $answer}){
        ...StudentQuestion_Model.Fragment.StudentQuestionFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("UpdateStudentQuestion");

module UpdateStudentQuestionMutation =
  ReasonApollo.CreateMutation(UpdateStudentQuestion);

let make = children => {
  ...component,
  render: _ =>
    <UpdateStudentQuestionMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~answer) => {
               let updateStudentQuestion =
                 UpdateStudentQuestion.make(~id, ~answer, ());
               mutation(
                 ~variables=updateStudentQuestion##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </UpdateStudentQuestionMutation>,
};