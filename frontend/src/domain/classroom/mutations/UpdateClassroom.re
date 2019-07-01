module MutationQuery = [%graphql
  {|
    mutation updateClassroom($id: ID!, $name: String!) {
      updateClassroom(classroom: {id: $id, name: $name}){
        ...Classroom_Model.Fragment.ClassroomFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("giveTestToClassroom");

module Mutation = ReasonApollo.CreateMutation(MutationQuery);

let make = children => {
  ...component,
  render: _ =>
    <Mutation>
      ...{
           (mutation, _ /* Result of your mutation */) =>
             children(~mutation=(~id, ~name) => {
               let mutationQuery = MutationQuery.make(~id, ~name, ());
               mutation(
                 ~variables=mutationQuery##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               );
             })
         }
    </Mutation>,
};