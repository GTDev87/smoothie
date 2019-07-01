module AddChoice = [%graphql
  {|
    mutation addChoice($id: ID!, $questionId: ID!, $text: String!) {
      addChoice(choice: {id: $id, questionId: $questionId, text: $text}){
        ...MultipleChoiceQuestion_Model.Fragment.MultipleChoiceQuestionFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddChoice");

module AddChoiceMutation = ReasonApollo.CreateMutation(AddChoice);

let make = children => {
  ...component,
  render: _ =>
    <AddChoiceMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~questionId, ~text) => {
               let addChoice = AddChoice.make(~id, ~questionId, ~text, ());
               mutation(
                 ~variables=addChoice##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </AddChoiceMutation>,
};