module AddTest = [%graphql
  {|
    mutation addTest($id: ID!, $name: String!, $description: String!, $teacherId: ID!) {
      addTest(test: { id: $id, name: $name, description: $description, teacherId: $teacherId}){
        ...Teacher_Model.Fragment.TeacherFields
      }
    }
  |}
];

let component = ReasonReact.statelessComponent("AddTest");

module AddTestMutation = ReasonApollo.CreateMutation(AddTest);

let make = (~id, ~name, ~description, ~teacherId, children) => {
  ...component,
  render: _ =>
    <AddTestMutation>
      ...{
           (mutation, _ /* Result of your mutation */) => {
             let newTest =
               AddTest.make(~id, ~name, ~description, ~teacherId, ());
             children(~addTest=() =>
               mutation(
                 ~variables=newTest##variables,
                 /* ~refetchQueries=[|"member"|], */
                 (),
               )
             );
           }
         }
    </AddTestMutation>,
};