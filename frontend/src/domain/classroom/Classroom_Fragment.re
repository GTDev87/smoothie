module Record = Classroom_Record;

module GraphFragment = [%graphql
  {|
     fragment classroomFields on Classroom {
       id
       name
       studentIds
       students{
         ...Student.Model.Fragment.Fields
       }
       testIds
       tests{
         ...Test.Model.Fragment.Fields
       }
       testAnalytics{
        ...TestAnalytics.Model.Fragment.Fields
       }
     }
   |}
];

include GraphFragment;
module Fields = GraphFragment.ClassroomFields;

let fragmentType = "Classroom";

let fromObject = (obj: Fields.t): Record._data => {
  id: obj##id,
  name: obj##name,
  studentIds:
    obj##students
    |> Belt.List.fromArray
    |> Belt.List.map(_, Student.Model.objectToId),
  testIds:
    obj##tests
    |> Belt.List.fromArray
    |> Belt.List.map(_, Test.Model.objectToId),
  testAnalyticsIds:
    obj##testAnalytics
    |> Belt.List.fromArray
    |> Belt.List.map(_, TestAnalytics.Model.objectToId),
};