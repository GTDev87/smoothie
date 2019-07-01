open Belt;

let css = Css.css;
let cx = Css.cx;
let tw = Css.tw;

type scoreObj = {
  .
  "nameList": array(string),
  "denom": string,
  "count": int,
};

let fullWidth =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~padding="2em",
    ~border="solid 1px black",
    ~borderRadius=".2em",
    (),
  );

let sideBySideClass = [%bs.raw {| css(tw`
  flex
  flex-start
  w-full
`)|}];

let labelClass = [%bs.raw {| css(tw`
  mr-12
`)|}];

let flexBetweenStyle =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="space-between", ());

let component = ReasonReact.statelessComponent("TestScoreGraph");

type dataType = {
  .
  "x": float,
  "y": float,
  "fill": string,
  "width": float,
  "height": float,
  "female": int,
  "name": string,
  "nameList": array(string),
  "denom": string,
  "count": int,
};

let make =
    (
      ~selectedTestAnalytics: TestAnalytics.Model.Record.t,
      ~normalized,
      ~updateTest,
      _children,
    ) => {
  ...component,
  render: _self => {
    let studentStatistics = (selectedTestAnalytics.data.mean, selectedTestAnalytics.data.standardDeviation);

    let test = MyNormalizr.Converter.Test.Remote.getRecordWithDefault(normalized, selectedTestAnalytics.data.testId, ());

    let max = selectedTestAnalytics.data.max;
    let min = selectedTestAnalytics.data.min;

    let studentScoreList2 = 
      selectedTestAnalytics.data.studentTestAnalyticsIds
      |> MyNormalizr.Converter.StudentTestAnalytics.idListToFilteredItems(_, MyNormalizr.Converter.StudentTestAnalytics.Remote.getRecord(normalized))
      |> Belt.List.map(_, (studentTestAnalytics) => (studentTestAnalytics.data.name, studentTestAnalytics.data.score));

    let stringListUnstartedStudents =
      selectedTestAnalytics.data.unstartedStudentIds
      |> MyNormalizr.Converter.Student.idListToFilteredItems(_, MyNormalizr.Converter.Student.Remote.getRecord(normalized))
      |> Belt.List.map(_, (student) => student.data.firstName)
      |> Utils.List.joinStringList(_, ", ");

    let median = selectedTestAnalytics.data.median;

    let isMasteryMode =
      test.local.overviewGraphDisplay == OverviewGraphDisplay.Mastery;

    let scoreHistogram =
      Utils.Stats.createScoreDenomHistogram(
        studentScoreList2,
        studentStatistics,
      );

    let categoryList =
      scoreHistogram
      |> Belt.List.reduce(_, [], (memo, ele) => (memo @ (ele.nameList |> Belt.List.map(_, (name) => (name, ele.denom)))))
      |> Belt.List.sort(_, ((_, a), (_, b)) => (Utils.Stats.demonStringToInt(b) - Utils.Stats.demonStringToInt(a)));

    let phi = selectedTestAnalytics.data.reliability;

    let finalGraphScores = studentScoreList2;

    <div style=fullWidth>
      <div style=flexBetweenStyle>
        <h1>
          {ReasonReact.string(isMasteryMode ? "Mastery View" : "Standard View")}
        </h1>
        <div>
          <Button
            onClick={
              () =>
                updateTest(Test.Action.ChangeGraphDisplay(isMasteryMode ? OverviewGraphDisplay.Normal : OverviewGraphDisplay.Mastery))
                |> ignore
            }
            theme=CTA>
            {
              ReasonReact.string(
                isMasteryMode ?
                  "Switch To Standard View" : "Switch To Mastery View",
              )
            }
          </Button>
        </div>
      </div>
      {
        isMasteryMode ?
          <Chart
            xLabel="Questions"
            yLabel="Test Takers"
            data={finalGraphScores |> Belt.List.toArray}
          /> :
          <Chart
            xLabel=" "
            data=(categoryList |> Belt.List.toArray)
            categorical=true binValues=(Utils.Stats.denoms |> Belt.List.toArray)
          />
          /* <Chart
            xLabel="Buckets"
            data={scoreHistogram |> Belt.List.toArray}
            binValues=Utils.Stats.histogramChoices
          /> */
      }
      <div className=sideBySideClass>
        <div className=labelClass>
          {ReasonReact.string("Number Students: ")}
          {ReasonReact.string(selectedTestAnalytics.data.numStudents |> string_of_int)}
        </div>
        
        <div className=labelClass>
          {ReasonReact.string("Max: ")}
          {max |> int_of_float |> string_of_int |> ReasonReact.string}
        </div>
        <div className=labelClass>
          {ReasonReact.string("Min: ")}
          {min |> int_of_float |> string_of_int |> ReasonReact.string}
        </div>
        <div className=labelClass>
          {ReasonReact.string("Mean: ")}
          {ReasonReact.string(selectedTestAnalytics.data.mean |> Utils.String.toStringWithPercision(_, 2))}
        </div>
        <div className=labelClass>
          {ReasonReact.string("Median: ")}
          {median |> int_of_float |> string_of_int |> ReasonReact.string}
        </div>
        {
          (!isMasteryMode)
            ? <div className=labelClass>
                {ReasonReact.string("Standard Deviation: ")}
                {ReasonReact.string(selectedTestAnalytics.data.standardDeviation |> Utils.String.toStringWithPercision(_, 3))}
              </div>
            : <div/>
        }
        {
          (!isMasteryMode)
            ? <div className=labelClass>
                {ReasonReact.string("Dependability: ")}
                {phi |> Utils.String.toStringWithPercision(_, 5) |> ReasonReact.string}
              </div>
            : <div/>
        }
        {((selectedTestAnalytics.data.unstartedStudentIds |> Belt.List.length) > 0) ?
          <div className=labelClass>
            {ReasonReact.string("Did not take test: ")}
            {stringListUnstartedStudents |> ReasonReact.string}
          </div> :
          <div/>
        }
      </div>
    </div>;
  },
};