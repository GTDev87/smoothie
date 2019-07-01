/* TODO not tested yet */
module GiveQuestionToTest = [%graphql
  {|
    mutation giveQuestionToTest($id: ID!, $studentTestId: ID!, $originalId: ID!) {
      giveQuestionToTest(studentQuestion: {id: $id, studentTestId: $studentTestId, originalId: $originalId}){
        ...StudentTest_Model.Fragment.StudentTestFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("GiveQuestionToTest");

module GiveQuestionToTestMutation =
  ReasonApollo.CreateMutation(GiveQuestionToTest);

let make = children => {
  ...component,
  render: _ =>
    <GiveQuestionToTestMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~originalId, ~studentTestId) => {
               let giveQuestionToTest =
                 GiveQuestionToTest.make(
                   ~id,
                   ~originalId,
                   ~studentTestId,
                   (),
                 );
               mutation(
                 ~variables=giveQuestionToTest##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </GiveQuestionToTestMutation>,
};