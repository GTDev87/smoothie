let component = ReasonReact.statelessComponent("TestTabbedSpreadsheets");

let css = Css.css;
let cx = Css.cx;
let tw = Css.tw;

let tabbedStyles =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~display="flex",
    ~justifyContent="center",
    (),
  );

let emptyDivStyle = ReactDOMRe.Style.make(~width="9.3em", ());

let tabTypeToComponent = (resultTabType, normalized, selectedTestAnalytics, updateTest) =>
  switch (resultTabType) {
  | ResultTab.Overview => <TestScoreGraph normalized selectedTestAnalytics updateTest />
  | ResultTab.Score => <TestAnswerKeySpreadsheet normalized selectedTestAnalytics updateTest />
  | ResultTab.Objective => <TestObjectiveSpreadsheet normalized selectedTestAnalytics />
  | ResultTab.Question => <TestQuestionDetailsSpreadsheet normalized selectedTestAnalytics />
  };

let testTabbedSpreadsheetClass = [%bs.raw {| css(tw`
  mr-8
`)|}];

let testTabbedSpreadsheetHeaderClass = [%bs.raw {| css(tw`
  flex
  mb-4
`)|}];

let testTabbedSpreadsheetHeaderSectionClass = [%bs.raw {| css(tw`
  w-1/3
`)|}];

let testTabbedSpreadsheetRawButtonClass = [%bs.raw {| css(tw`
  flex
  justify-end
`)|}];

let make =
  (
    ~selectedTestAnalytics: TestAnalytics.Model.Record.t,
    ~students,
    ~headerSection: ReasonReact.reactElement,
    ~updateTest,
    ~normalized,
    _children,
  ) => {
  ...component,
  render: _self => {
    normalized
    |> MyNormalizr.Converter.Test.Remote.getRecord(_, selectedTestAnalytics.data.testId)
    |> Belt.Option.mapWithDefault(_, <div/>, (selectedTest) => {
      <div className=testTabbedSpreadsheetClass>
        <div className=testTabbedSpreadsheetHeaderClass>
          <div className=testTabbedSpreadsheetHeaderSectionClass>headerSection</div>
          <div className=testTabbedSpreadsheetHeaderSectionClass>
            <TabNav>
              <div style=tabbedStyles>
                {
                  Belt.List.map(ResultTab.all, resultTabType => {
                    let resultTabString = resultTabType |> ResultTab.resultTypeTabToString;
                    <TabNavItem key=resultTabString>
                      <TabNavLink
                        className={selectedTest.local.resultTab == resultTabType ?  "active" : ""}
                        onClick={() => updateTest(Test.Action.SelectResultTab(resultTabType)) |> ignore
                      }>
                        {ReasonReact.string(resultTabString)}
                      </TabNavLink>
                    </TabNavItem>;
                  })
                  |> Utils.ReasonReact.listToReactArray
                }
              </div>
            </TabNav>
          </div>
          <div className=cx(testTabbedSpreadsheetHeaderSectionClass, testTabbedSpreadsheetRawButtonClass)>
            <TestRawDownloadButton normalized test=selectedTest students />
          </div>
        </div>
        <Tabs activeTab={ResultTab.resultTypeTabToString(selectedTest.local.resultTab)}>
          {
            ResultTab.all
            |> Belt.List.keep(_, resultTabType => resultTabType == selectedTest.local.resultTab)  /* monkey patch... should do fuller solution*/
            |> Belt.List.map(_, resultTabType => {
                let resultTabString = resultTabType |> ResultTab.resultTypeTabToString;
                <Tab key=resultTabString tabId=resultTabString>
                  {tabTypeToComponent(resultTabType, normalized, selectedTestAnalytics, updateTest)}
                </Tab>
              })
            |> Utils.ReasonReact.listToReactArray
          }
        </Tabs>
      </div>
    })
  }
};