module Record = AnswerKeyAnalytics_Record;

module GraphFragment = [%graphql
  {|
    fragment answerKeyAnalyticsFields on AnswerKeyAnalytics {
      id
      questionText
      objectiveText
      discrimination
      studentQuestionAnswerIds
      answerKeyId
      mastersIds
      developingIds
      histogram{
        correct
        text
        freq
      }
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.AnswerKeyAnalyticsFields;

let fragmentType = "AnswerKeyAnalytics";

let fromObject = (obj: Fields.t): Record._data => {
  {
    id: obj##id,
    questionText: obj##questionText,
    objectiveText: obj##objectiveText,
    discrimination: obj##discrimination,
    studentQuestionAnswerIds:
      obj##studentQuestionAnswerIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, StudentAnswerKey.Model.idToTypedId),
    answerKeyId: QuestionAnswerKey.Model.idToTypedId(obj##answerKeyId),
    masterIds:
      obj##mastersIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, Student.Model.idToTypedId),
    developingIds:
      obj##developingIds
      |> Belt.List.fromArray
      |> Belt.List.map(_, Student.Model.idToTypedId),
    histogram:
      obj##histogram
      |> Belt.List.fromArray
      |> Belt.List.map(_, (hisogramValue): Record.histogramValue  => {
        text: hisogramValue##text,
        freq: hisogramValue##freq,
        correct: hisogramValue##correct,
      }),
  }
};