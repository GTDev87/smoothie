module UpdateQuestionType = [%graphql
  {|
    mutation updateQuestionType($id: ID!, $testId: ID!, $questionType: QuestionType!) {
      updateQuestionType(question: {id: $id, testId: $testId, questionType: $questionType}){
        ...Test_Model.Fragment.TestFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("UpdateQuestionType");

module UpdateQuestionTypeMutation =
  ReasonApollo.CreateMutation(UpdateQuestionType);

let make = children => {
  ...component,
  render: _ =>
    <UpdateQuestionTypeMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~testId, ~questionType) => {
               let updateQuestionType =
                 UpdateQuestionType.make(~id, ~testId, ~questionType, ());
               mutation(
                 ~variables=updateQuestionType##variables,
                 ~refetchQueries=[|"member"|],
                 (),
               );
             })
         }
    </UpdateQuestionTypeMutation>,
};