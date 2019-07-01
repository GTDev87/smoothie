module AddAnswerKey = [%graphql
  {|
    mutation addAnswerKey($id: ID!, $questionId: ID!) {
      addAnswerKey(questionAnswerKey: {id: $id, questionId: $questionId}){
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

let component = ReasonReact.statelessComponent("AddAnswerKey");

module AddAnswerKeyMutation = ReasonApollo.CreateMutation(AddAnswerKey);

let make = children => {
  ...component,
  render: _ =>
    <AddAnswerKeyMutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~questionId) => {
               let addAnswerKey = AddAnswerKey.make(~id, ~questionId, ());
               mutation(
                 ~variables=addAnswerKey##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </AddAnswerKeyMutation>,
};