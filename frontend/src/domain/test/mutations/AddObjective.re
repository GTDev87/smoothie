module AddObjective = [%graphql
  {|
    mutation addObjective($objectiveId: ID!, $testId: ID!, $id: ID!) {
      addObjective(testObjective: {id: $id, testId: $testId, objectiveId: $objectiveId}){
        ...Test_Model.Fragment.TestFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddObjective");

module AddObjectiveMutation = ReasonApollo.CreateMutation(AddObjective);

let make = children => {
  ...component,
  render: _ =>
    <AddObjectiveMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~objectiveId, ~testId) => {
               let addObjective =
                 AddObjective.make(
                   ~id=UUID.generateUUID(),
                   ~testId,
                   ~objectiveId,
                   (),
                 );
               mutation(
                 ~variables=addObjective##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </AddObjectiveMutation>,
};