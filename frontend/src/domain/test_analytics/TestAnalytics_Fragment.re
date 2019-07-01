module Record = TestAnalytics_Record;

module GraphFragment = [%graphql
  {|
    fragment testAnalyticsFields on TestAnalytics {
      id
      totalScore
      min
      max
      reliability
      mean
      median
      standardDeviation
      testId
      startedStudentIds
      unstartedStudentIds
      studentTestAnalytics {
        ...StudentTestAnalytics.Model.Fragment.Fields
      }
      answerKeyAnalytics {
        ...AnswerKeyAnalytics.Model.Fragment.Fields
      }
      objectiveAnalytics {
        ...ObjectiveAnalytics.Model.Fragment.Fields
      }
      numStudents
      testName
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.TestAnalyticsFields;

let fragmentType = "TestAnalytics";

let fromObject = (obj: Fields.t): Record._data => {
  id: obj##id,
  totalScore: obj##totalScore,
  testName: obj##testName,
  min: obj##min,
  max: obj##max,
  reliability: obj##reliability,
  mean: obj##mean,
  median: obj##median,
  standardDeviation: obj##standardDeviation,
  testId: Test.Model.idToTypedId(obj##testId),
  numStudents: obj##numStudents,
  studentTestAnalyticsIds:
    obj##studentTestAnalytics
    |> Belt.List.fromArray
    |> Belt.List.map(_, StudentTestAnalytics.Model.objectToId),
  startedStudentIds:
    obj##startedStudentIds
    |> Belt.List.fromArray
    |> Belt.List.map(_, Student.Model.idToTypedId),
  unstartedStudentIds:
    obj##unstartedStudentIds
    |> Belt.List.fromArray
    |> Belt.List.map(_, Student.Model.idToTypedId),
  answerKeyAnalyticsIds:
    obj##answerKeyAnalytics
    |> Belt.List.fromArray
    |> Belt.List.map(_, AnswerKeyAnalytics.Model.objectToId),
  objectiveAnalyticsIds:
    obj##objectiveAnalytics
    |> Belt.List.fromArray
    |> Belt.List.map(_, ObjectiveAnalytics.Model.objectToId),
};