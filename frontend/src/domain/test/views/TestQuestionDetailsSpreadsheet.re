open Belt;

type histogramWithBool = list((string, bool, int));

type questionDetailsObj = {
  .
  "questionNumber": string,
  "questionObjective": string,
  "answerHistogram": histogramWithBool,
  "correctStudents": list(string),
  "incorrectStudents": list(string),
  "discrimination": float,
};

let defaultDetailsObj = {
  "questionNumber": "",
  "questionObjective": "",
  "answerHistogram": [],
  "correctStudents": [],
  "incorrectStudents": [],
  "discrimination": 0.0,
};

let correctAnswerStyle =
  ReactDOMRe.Style.make(~fontSize="1em", ~color=Colors.correct, ());

let incorrectAnswerStyle =
  ReactDOMRe.Style.make(~fontSize="1em", ~color=Colors.incorrect, ());

let infoStyles =
  ReactDOMRe.Style.make(~width="100%", ~marginBottom="1em", ());

let questionColumns = (
  questionDetailsObjs: list(questionDetailsObj),
) : list(DataGrid.column(questionDetailsObj)) => [
  {
    _Header: "% Class Correct",
    accessor: String(""),
    id: None,
    minWidth: None,
    maxWidth: None,
    _Cell:
      Some(
        (cellInfo: DataGrid.cellInfo(Js.Dict.t(string))) => {
          let questionDetail =
            questionDetailsObjs
            |> Belt.List.get(_, cellInfo##index)
            |> Belt.Option.getWithDefault(_, defaultDetailsObj);

          let countList: list(PieChart.freqData) =
            Belt.List.map(
              questionDetail##answerHistogram,
              ((answerText: string, correct, count: int)) =>
              ({freq: count |> float_of_int, name: answerText, correct}: PieChart.freqData));

          <div>
            <VxParentSize>
              {parent => <PieChart data=countList width=parent##width />}
            </VxParentSize>
            <div>
              {ReasonReact.string("Discrimination : " ++ (questionDetail##discrimination |> Utils.String.toStringWithPercision(_, 3)))}
            </div>
          </div>;
        },
      ),
    headerClassName: None,
  },
  {
    _Header: "Question",
    accessor: String("questionNumber"),
    id: None,
    minWidth: None,
    maxWidth: Some(200),
    _Cell:
      Some(
        (cellInfo: DataGrid.cellInfo(Js.Dict.t(string))) => {
          let questionDetail =
            questionDetailsObjs
            |> Belt.List.get(_, cellInfo##index)
            |> Belt.Option.getWithDefault(_, defaultDetailsObj);

          <TestTooltip
            key=questionDetail##questionNumber
            testText=questionDetail##questionNumber
            maxTextLength=100
          />;
        },
      ),
    headerClassName: None,
  },
  {
    _Header: "Student Performance",
    accessor: String("studentPerformance"),
    id: None,
    minWidth: None,
    maxWidth: None,
    _Cell:
      Some(
        (cellInfo: DataGrid.cellInfo(Js.Dict.t(string))) => {
          let questionDetail =
            questionDetailsObjs
            |> Belt.List.get(_, cellInfo##index)
            |> Belt.Option.getWithDefault(_, defaultDetailsObj);

          <div>
            {
              Belt.List.length(questionDetail##correctStudents) != 0 ?
                <StudentMasteryList
                  listName="Masters"
                  correct=true
                  studentList=questionDetail##correctStudents
                /> :
                <div />
            }
            {
              Belt.List.length(questionDetail##incorrectStudents) != 0 ?
                <StudentMasteryList
                  listName="Developing"
                  correct=false
                  studentList=questionDetail##incorrectStudents
                /> :
                <div />
            }
          </div>;
        },
      ),
    headerClassName: None,
  },
  {
    _Header: "objective",
    accessor: String("questionObjective"),
    id: None,
    minWidth: None,
    maxWidth: Some(100),
    _Cell: None,
    headerClassName: None,
  },
];

let createQuestionAnalyticsDetails = (normalized, testAnalytics: TestAnalytics.Model.Record.t) => {
  testAnalytics.data.answerKeyAnalyticsIds
  |> MyNormalizr.Converter.AnswerKeyAnalytics.idListToFilteredItems(_, MyNormalizr.Converter.AnswerKeyAnalytics.Remote.getRecord(normalized))
  |> Belt.List.mapWithIndex(_, (idx, answerKeyAnalytics) => {
    ({
      "questionNumber":
        ("Q." ++ (idx + 1 |> string_of_int))
        ++ " "
        ++ answerKeyAnalytics.data.questionText,
      "questionObjective": answerKeyAnalytics.data.objectiveText,
      "answerHistogram":
        answerKeyAnalytics.data.histogram
        |> Belt.List.map(_, (histogramValue) => (histogramValue.text, histogramValue.correct, histogramValue.freq)),
      "correctStudents":
        answerKeyAnalytics.data.masterIds
        |> MyNormalizr.Converter.Student.idListToFilteredItems(_, MyNormalizr.Converter.Student.Remote.getRecord(normalized))
        |> Belt.List.map(_, (student) => student.data.firstName ++ " " ++ student.data.lastName),
      "incorrectStudents":
        answerKeyAnalytics.data.developingIds
        |> MyNormalizr.Converter.Student.idListToFilteredItems(_, MyNormalizr.Converter.Student.Remote.getRecord(normalized))
        |> Belt.List.map(_, (student) => student.data.firstName ++ " " ++ student.data.lastName),
      "discrimination": answerKeyAnalytics.data.discrimination,
    });
  })
}

let component =
  ReasonReact.statelessComponent("TestQuestionDetailsSpreadsheet");

let make = (
  ~selectedTestAnalytics: TestAnalytics.Model.Record.t,
  ~normalized,
  _children
) => {
  ...component,
  render: _self => {
    let questionDetailsObjs: list(questionDetailsObj) =
      createQuestionAnalyticsDetails(normalized, selectedTestAnalytics);

    /* let masters =
    let developing =  */

    <div>
      /* <InfoBox info="Words and words and words and words" style=infoStyles /> */
      <DataGrid
        pageSize={List.length(questionDetailsObjs)}
        columns={questionColumns(questionDetailsObjs)}
        dataList=questionDetailsObjs
      />
    </div>;
  },
};