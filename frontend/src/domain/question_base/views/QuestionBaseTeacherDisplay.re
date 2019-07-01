let component = ReasonReact.statelessComponent("QuestionBaseTeacherDisplay");
let css = Css.css;
let tw = Css.tw;
let questionBaseTeacherDisplayClass = [%bs.raw {| css(tw`
`)|}];

let questionBaseTeacherDisplayCheckClass = [%bs.raw {| css(tw` text-green `)|}];
let questionBaseTeacherDisplayXClass = [%bs.raw {| css(tw` text-red `)|}];

let checkIfTrue = check =>
  check
    ? <CheckIcon className=questionBaseTeacherDisplayCheckClass />
    : <TimesIcon className=questionBaseTeacherDisplayXClass />;
let make =
    (
      ~data as questionBase : QuestionBase.Model.Record.t,
      ~testNotes: option(string),
      _children,
    ) => {
  ...component,
  render: _self => {
    let baseComponent =
        <div className=questionBaseTeacherDisplayClass>
          <Row>
            <Col xs=2>
              <h3 className="h3">
                {ReasonReact.string("Solution:")}
              </h3>
            </Col>
            <Col xs=10>
              {ReasonReact.string(questionBase.data.solution)}
            </Col>
          </Row>
          <div>
            <LabelContent
              labelContentList=[
                ("Auto Score", checkIfTrue(questionBase.data.autoScore)),
                ("Forced Response", checkIfTrue(questionBase.data.forcedResponse)),
              ]
            />
          </div>
        </div>
    Belt.Option.mapWithDefault(testNotes, baseComponent, (notes) =>
      <Tooltip tooltipText=notes>baseComponent</Tooltip>);
    
  }
};