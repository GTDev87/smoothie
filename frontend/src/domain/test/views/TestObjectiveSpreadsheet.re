open Belt;

type objectiveDetailsObj = {
  .
  "objective": string,
  "classCorrect": string,
  "totalNumQuestions": int,
  "questionsWithObjective": list((QuestionBase.Model.Record.t, int)),
  "studentObjectivesCorrectWithStatistics": (list(Student.Model.Record.t), list(Student.Model.Record.t)),
};

let defaultObjectiveDetailsObj = {
  "objective": "",
  "classCorrect": "",
  "totalNumQuestions": 0,
  "questionsWithObjective": [],
  "studentObjectivesCorrectWithStatistics": ([], []),
};

let infoStyles =
  ReactDOMRe.Style.make(~width="100%", ~marginBottom="1em", ());

let makeObjectiveColumns =
    objectiveObjs: list(DataGrid.column(objectiveDetailsObj)) => [
  (
    {
      _Header: "Objective",
      accessor: String("objective"),
      id: None,
      minWidth: None,
      maxWidth: Some(200),
      headerClassName: None,
      _Cell:
        Some(
          (cellInfo: DataGrid.cellInfo(objectiveDetailsObj)) => {
            let objectiveObj =
              objectiveObjs
              |> Belt.List.get(_, cellInfo##index)
              |> Belt.Option.getWithDefault(_, defaultObjectiveDetailsObj);

            <div>
              <h3> {ReasonReact.string(objectiveObj##objective)} </h3>
              <div> {ReasonReact.string("Proportion Correct")} </div>
              <h1> {ReasonReact.string(objectiveObj##classCorrect)} </h1>
            </div>;
          },
        ),
    }:
      DataGrid.column(objectiveDetailsObj)
  ),
  (
    {
      _Header: "Related Questions to Objective",
      accessor: String("questionsWithObjective"),
      id: None,
      _Cell:
        Some(
          (cellInfo: DataGrid.cellInfo(objectiveDetailsObj)) => {
            let objectiveObj =
              objectiveObjs
              |> Belt.List.get(_, cellInfo##index)
              |> Belt.Option.getWithDefault(_, defaultObjectiveDetailsObj);

            let questionsWithIndex = objectiveObj##questionsWithObjective;

            <div>
              {
                questionsWithIndex
                |> Belt.List.map(
                     _, ((questionBase: QuestionBase.Model.Record.t, idx)) =>
                     <TestTooltip
                       key={questionBase.data.id}
                       testText={questionBase.data.text}
                       testNumber=(questionBase.data.questionNumber)
                     />
                   )
                |> Utils.ReasonReact.listToReactArray
              }
            </div>;
          },
        ),
      minWidth: None,
      maxWidth: None,
      headerClassName: None,
    }:
      DataGrid.column(objectiveDetailsObj)
  ),
  (
    {
      _Header: "",
      accessor: String("studentObjectivesCorrectWithStatistics"),
      id: None,
      _Cell:
        Some(
          (cellInfo: DataGrid.cellInfo(objectiveDetailsObj)) => {
            let objectiveObj =
              objectiveObjs
              |> Belt.List.get(_, cellInfo##index)
              |> Belt.Option.getWithDefault(_, defaultObjectiveDetailsObj);

            let (masters, developing) = objectiveObj##studentObjectivesCorrectWithStatistics;

            <div>
              <StudentMasteryList
                listName="Masters"
                correct=true
                studentList=(masters |> Belt.List.map(_, (student: Student.Model.Record.t) => student.data.firstName))
              />
              <StudentMasteryList
                listName="Developing"
                correct=false
                studentList=(developing |> Belt.List.map(_, (student: Student.Model.Record.t) => student.data.firstName))
              />
            </div>;
          },
        ),
      minWidth: None,
      maxWidth: None,
      headerClassName: None,
    }:
      DataGrid.column(objectiveDetailsObj)
  ),
];

let calcObjectiveStatistics = (values: list(float)) => (
  Utils.Stats.getMean(values),
  Utils.Stats.getStdDeviation(values),
);

let toAnalyticsObjectiveObj = (normalized, objectiveAnalytics: ObjectiveAnalytics.Model.Record.t) => {
  {
    "objective": objectiveAnalytics.data.objectiveName,
    "totalNumQuestions": objectiveAnalytics.data.questionIds |> Belt.List.length,
    "classCorrect": (((objectiveAnalytics.data.percentageCorrect *. 100.) |> Utils.String.toStringWithPercision(_, 2)) ++ "%"),
    "questionsWithObjective":
      objectiveAnalytics.data.questionIds
      |> MyNormalizr.Converter.QuestionBase.idListToFilteredItems(_, MyNormalizr.Converter.QuestionBase.Remote.getRecord(normalized))
      |> Belt.List.mapWithIndex(_, (idx, questionBase) => (questionBase, idx)),
    "studentObjectivesCorrectWithStatistics": (
      objectiveAnalytics.data.masterStudentIds
      |> MyNormalizr.Converter.Student.idListToFilteredItems(_, MyNormalizr.Converter.Student.Remote.getRecord(normalized)),
      objectiveAnalytics.data.developingStudentIds
      |> MyNormalizr.Converter.Student.idListToFilteredItems(_, MyNormalizr.Converter.Student.Remote.getRecord(normalized)),
    ),
  };
}

let toObjectiveObjs =
    (
      normalized,
      selectedTestAnalytics: TestAnalytics.Model.Record.t,
    )
    : list(objectiveDetailsObj) => {
  selectedTestAnalytics.data.objectiveAnalyticsIds
  |> MyNormalizr.Converter.ObjectiveAnalytics.idListToFilteredItems(_, MyNormalizr.Converter.ObjectiveAnalytics.Remote.getRecord(normalized))
  |> Belt.List.map(_, toAnalyticsObjectiveObj(normalized));
};

let component = ReasonReact.statelessComponent("TestObjectiveSpreadsheet");

let make = (
  ~selectedTestAnalytics: TestAnalytics.Model.Record.t,
  ~normalized,
  _children
) => {
  ...component,
  render: _self => {
    let objectiveObjs = toObjectiveObjs(normalized, selectedTestAnalytics);

    <div>
      /* <InfoBox info="Words and words and words and words" style=infoStyles /> */
      <DataGrid
        pageSize={List.length(objectiveObjs)}
        columns={makeObjectiveColumns(objectiveObjs)}
        dataList=objectiveObjs
      />
    </div>;
  },
};