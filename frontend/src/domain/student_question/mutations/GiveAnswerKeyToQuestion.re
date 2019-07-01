/* TODO not tested yet */
module GiveAnswerKeyToQuestion = [%graphql
  {|
    mutation giveAnswerKeyToQuestion($id: ID!, $studentQuestionId: ID!, $originalId: ID!) {
      giveAnswerKeyToQuestion(studentAnswerKey: {id: $id, studentQuestionId: $studentQuestionId, originalId: $originalId}){
        ...StudentQuestion_Model.Fragment.StudentQuestionFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("GiveAnswerKeyToQuestion");

module GiveAnswerKeyToQuestionMutation =
  ReasonApollo.CreateMutation(GiveAnswerKeyToQuestion);

let make = children => {
  ...component,
  render: _ =>
    <GiveAnswerKeyToQuestionMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~originalId, ~studentQuestionId) => {
               let giveAnswerKeyToQuestion =
                 GiveAnswerKeyToQuestion.make(
                   ~id,
                   ~originalId,
                   ~studentQuestionId,
                   (),
                 );
               mutation(
                 ~variables=giveAnswerKeyToQuestion##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </GiveAnswerKeyToQuestionMutation>,
};