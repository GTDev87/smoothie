module UpdateStimulus = [%graphql
  {|
    mutation updateStimulus($id: ID!, $text: String!) {
      updateStimulus(stimulus: {id: $id, text: $text}){
        ...Stimulus_Model.Fragment.StimulusFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("UpdateStimulus");

module UpdateStimulusMutation = ReasonApollo.CreateMutation(UpdateStimulus);

let make = children => {
  ...component,
  render: _ =>
    <UpdateStimulusMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~text) => {
               let updateStimulus = UpdateStimulus.make(~id, ~text, ());
               mutation(
                 ~variables=updateStimulus##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </UpdateStimulusMutation>,
};