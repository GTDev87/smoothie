module Record = ObjectiveAnalytics_Record;

module GraphFragment = [%graphql
  {|
    fragment objectiveAnalyticsFields on ObjectiveAnalytics {
      id
      objectiveName
      percentageCorrect
      questionIds
      masterStudentIds
      developingStudentIds
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.ObjectiveAnalyticsFields;

let fragmentType = "ObjectiveAnalytics";

let fromObject = (obj: Fields.t): Record._data => {
  {
    id: obj##id,
    objectiveName: obj##objectiveName,
    percentageCorrect: obj##percentageCorrect,
    questionIds:
      obj##questionIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, QuestionBase.Model.idToTypedId),
    masterStudentIds:
      obj##masterStudentIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, Student.Model.idToTypedId),
    developingStudentIds:
      obj##developingStudentIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, Student.Model.idToTypedId),
  }
};