module AddStimulus = [%graphql
  {|
    mutation addStimulus($id: ID!, $questionId: ID!) {
      addStimulus(stimulus: {id: $id, questionId: $questionId}){
        ...on LongAnswerQuestion {
          ...LongAnswerQuestion.Model.Fragment.Fields
        }
        ...on MultipleChoiceQuestion {
          ...MultipleChoiceQuestion.Model.Fragment.Fields
        }
        ...on TrueFalseQuestion {
          ...TrueFalseQuestion.Model.Fragment.Fields
        }
        ...on FillInTheBlankQuestion {
          ...FillInTheBlankQuestion.Model.Fragment.Fields
        }
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddStimulus");

module AddStimulusMutation = ReasonApollo.CreateMutation(AddStimulus);

let make = children => {
  ...component,
  render: _ =>
    <AddStimulusMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~questionId) => {
               let addStimulus = AddStimulus.make(~id, ~questionId, ());
               mutation(
                 ~variables=addStimulus##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </AddStimulusMutation>,
};