module UpdateQuestionAnswerKey = [%graphql
  {|
    mutation updateQuestionAnswerKey($id: ID!, $score: Float!, $objectiveId: ID) {
      updateQuestionAnswerKey(questionAnswerKey: {id: $id, score: $score, objectiveId: $objectiveId}){
        ...QuestionAnswerKey_Model.Fragment.QuestionAnswerKeyFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("UpdateQuestionAnswerKey");

module UpdateQuestionAnswerKeyMutation =
  ReasonApollo.CreateMutation(UpdateQuestionAnswerKey);

let make = children => {
  ...component,
  render: _ =>
    <UpdateQuestionAnswerKeyMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~score, ~objectiveId=?, ()) => {
               let updateQuestionAnswerKey =
                 switch (objectiveId) {
                 | Some(Some(objectiveId)) =>
                   UpdateQuestionAnswerKey.make(
                     ~id,
                     ~score,
                     ~objectiveId=objectiveId |> Objective.Model.getUUIDFromId,
                     (),
                   )
                 | _ => UpdateQuestionAnswerKey.make(~id, ~score, ())
                 };

               mutation(
                 ~variables=updateQuestionAnswerKey##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </UpdateQuestionAnswerKeyMutation>,
};