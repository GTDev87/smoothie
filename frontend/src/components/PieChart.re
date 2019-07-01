let component = ReasonReact.statelessComponent("PieChart");

type freqData = {
  freq: float,
  name: string,
  correct: bool,
};

let chartStyles =
  ReactDOMRe.Style.make(~display="flex", ~alignItems="flex-start", ());

let correctAnswerStyle =
  ReactDOMRe.Style.make(~fontSize="1em", ~color=Colors.correct, ());

let incorrectAnswerStyle =
  ReactDOMRe.Style.make(~fontSize="1em", ~color=Colors.incorrect, ());

let chartWidth = 180.;
let chartHeight = 180.;

let sumFreq = list =>
  Belt.List.reduce(list, 0., (memo, freqData) => memo +. freqData.freq);

let make = (~data: list(freqData), ~width=?, _children) => {
  ...component,
  render: _self => {
    let correctColorScale = Scale.singleHueScaleFactory("green");
    let incorrectColorScale = Scale.singleHueScaleFactory("red");

    let piePortion = 0.6;
    let correctAnswers = Belt.List.keep(data, freqData => freqData.correct);
    let incorrectAnswers = Belt.List.keep(data, freqData => !freqData.correct);

    let total = sumFreq(data);
    let totalCorrect = sumFreq(correctAnswers);
    let totalIncorrect = sumFreq(incorrectAnswers);

    let percentageCalc = datum => 100. *. (datum.freq /. total);

    <div style=chartStyles>
      <RadialChart
        ariaLabel="This is a radial-chart chart of..."
        margin={"top": 0, "left": 0, "bottom": 0, "right": 0}
        width={Belt.Option.getWithDefault(width, chartWidth) *. piePortion}
        height=chartHeight
        renderTooltip={
          (pieData: RadialChart.pieData(freqData)) => {
            let datum = pieData##datum;
            let fraction = pieData##fraction;
            <div>
              <div> <strong> {ReasonReact.string(datum.name)} </strong> </div>
              <div>
                {fraction *. 100. |> string_of_float |> ReasonReact.string}
              </div>
            </div>;
          }
        }>
        <ArcSeries
          pieValue={d => d.freq}
          data={data |> Belt.List.toArray}
          label={
            arc =>
              (
                arc##data
                |> percentageCalc
                |> Js.Float.toFixedWithPrecision(_, ~digits=2)
              )
              ++ "%"
          }
          labelComponent={<ArcLabel />}
          innerRadius={_r => 0.}
          outerRadius={r => r *. 0.6}
          labelRadius={r => r *. 0.47}
          stroke="#fff"
          strokeWidth=1.5
          fill={
            arc => {
              let data = arc##data;
              data.correct ?
                correctColorScale(data) : incorrectColorScale(data);
            }
          }
        />
      </RadialChart>
      <div>
        {
          Belt.List.length(correctAnswers) !== 0 ?
            <div>
              <h3 style=correctAnswerStyle>
                {
                  ReasonReact.string(
                    "Correct Answers "
                    ++ (
                      100.
                      *. totalCorrect
                      /. total
                      |> Js.Float.toFixedWithPrecision(_, ~digits=1)
                    )
                    ++ "%",
                  )
                }
              </h3>
              <LegendOrdinal
                direction="column"
                domain={correctAnswers |> Belt.List.toArray}
                scale=correctColorScale
                shape="circle"
                fill={
                  (arc: LegendOrdinal.datumType(freqData)) =>
                    correctColorScale(arc##datum)
                }
                labelFormat={
                  label =>
                    label
                    |> percentageCalc
                    |> Js.Float.toFixedWithPrecision(_, ~digits=1)
                    |> (percentage => percentage ++ "%: " ++ label.name)
                }
              />
              <br />
            </div> :
            <div />
        }
        {
          Belt.List.length(incorrectAnswers) !== 0 ?
            <div>
              <h3 style=incorrectAnswerStyle>
                {
                  ReasonReact.string(
                    "Incorrect Answers "
                    ++ (
                      100.
                      *. totalIncorrect
                      /. total
                      |> Js.Float.toFixedWithPrecision(_, ~digits=1)
                    )
                    ++ "%",
                  )
                }
              </h3>
              <LegendOrdinal
                direction="column"
                domain={incorrectAnswers |> Belt.List.toArray}
                scale=incorrectColorScale
                shape="circle"
                fill={
                  (arc: LegendOrdinal.datumType(freqData)) =>
                    incorrectColorScale(arc##datum)
                }
                labelFormat={
                  label =>
                    label
                    |> percentageCalc
                    |> Js.Float.toFixedWithPrecision(_, ~digits=1)
                    |> (percentage => percentage ++ "%: " ++ label.name)
                }
              />
              <br />
            </div> :
            <div />
        }
      </div>
    </div>;
  },
};