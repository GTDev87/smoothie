module AddQuestion = [%graphql
  {|
    mutation addQuestion($id: ID!, $testId: ID!) {
      addQuestion(question: {id: $id, testId: $testId}){
        ...Test_Model.Fragment.TestFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddQuestion");

module AddQuestionMutation = ReasonApollo.CreateMutation(AddQuestion);

let make = children => {
  ...component,
  render: _ =>
    <AddQuestionMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~testId) => {
               let newStudent = AddQuestion.make(~id, ~testId, ());
               mutation(
                 ~variables=newStudent##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </AddQuestionMutation>,
};