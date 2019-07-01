module Record = StudentTestAnalytics_Record;

module GraphFragment = [%graphql
  {|
    fragment studentTestAnalyticsFields on StudentTestAnalytics {
      id
      studentTestId
      percentage
      name
      score
      studentAnswerKeyIds
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.StudentTestAnalyticsFields;

let fragmentType = "StudentTestAnalytics";

let fromObject = (obj: Fields.t): Record._data => {
  {
    id: obj##id,
    name: obj##name,
    score: obj##score,
    percentage: obj##percentage,
    studentTestId: StudentTest.Model.idToTypedId(obj##studentTestId),
    studentAnswerKeyIds:
      obj##studentAnswerKeyIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, StudentAnswerKey.Model.idToTypedId),
  }
};