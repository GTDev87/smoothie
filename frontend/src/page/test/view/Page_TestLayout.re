let component = ReasonReact.statelessComponent("Page_StudentTestLayout");

let make = (~testId: string, _children) => {
  ...component,
  render: _self =>
    <Page_Test_Container testId errorComponent={<LoginLayout />}>
      ...{
           (~testId) =>
             switch (testId) {
             | Some(testId) =>
               <Test.Container id=testId>
                 ...(
                      (test) => {
                        <NormalizrInit records=[Test.Record.Record(test)]>
                          ...{
                               (~normalized, ~updateNormalizr) =>
                                 <TestPreview
                                   testId={Schema.Test.stringToId(testId)}
                                   normalized
                                 />
                             }
                        </NormalizrInit>;
                      }
                    )
               </Test.Container>
             | _ => <div />
             }
         }
    </Page_Test_Container>,
};