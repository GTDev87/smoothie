/* This doesn't really belong here but I'll add it it belongs with queries*/

let component = ReasonReact.statelessComponent("GradesContainer");

module GradesQuery = ReasonApollo.CreateQuery(Query.Grades.M);

let make = children => {
  ...component,
  render: _ => {
    let gradesQuery = Query.Grades.M.make();

    <GradesQuery variables=gradesQuery##variables>
      ...{
           ({result}) =>
             switch (result) {
             | Loading => <div> {ReasonReact.string("Loading")} </div>
             | Error(error) =>
               <div> {ReasonReact.string(error##message)} </div>
             | Data(response) => children(~grades=response##grades)
             }
         }
    </GradesQuery>;
  },
};