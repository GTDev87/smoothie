module MutationQuery = [%graphql
  {|
    mutation giveTestToClassroom($testId: ID!, $classroomId: ID!) {
      giveTestToClassroom(classroomTest: {testId: $testId, classroomId: $classroomId}){
        ...Classroom_Model.Fragment.ClassroomFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("GiveTestToClassroom");

module Mutation = ReasonApollo.CreateMutation(MutationQuery);

let make = children => {
  ...component,
  render: _ =>
    <Mutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~testId, ~classroomId) => {
               let mutationQuery =
                 MutationQuery.make(~testId, ~classroomId, ());
               mutation(
                 ~variables=mutationQuery##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </Mutation>,
};