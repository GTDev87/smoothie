module CreateUpdateObjective = [%graphql
  {|
    mutation createUpdateObjective($id: ID!, $text: String!) {
      createUpdateObjective(objective: {id: $id, text: $text}){
        ...Objective_Model.Fragment.ObjectiveFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("CreateUpdateObjective");

module CreateUpdateObjectiveMutation =
  ReasonApollo.CreateMutation(CreateUpdateObjective);

let make = children => {
  ...component,
  render: _ =>
    <CreateUpdateObjectiveMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~text) => {
               let createObjective =
                 CreateUpdateObjective.make(~id, ~text, ());
               mutation(
                 ~variables=createObjective##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </CreateUpdateObjectiveMutation>,
};