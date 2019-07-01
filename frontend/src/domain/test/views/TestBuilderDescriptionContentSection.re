let component =
  ReasonReact.statelessComponent("TestBuilderDescriptionContentSection");

let css = Css.css;
let tw = Css.tw;

let testDescriptionText = [%bs.raw {| css(tw`
  font-semibold
`)|}
];

let testDescriptionTextStyle =
  ReactDOMRe.Style.make(
    ~backgroundColor="inherit",
    ~height="100%",
    ~width="100%",
    ~wordWrap="break-word",
    ~wordBreak="break-all",
    (),
  );

let make = (~test: Test.Model.Record.t, ~updateTest, _children) => {
  ...component,
  render: _self =>
    <Test.Mutation.UpdateTest>
      ...{
           (~mutation) =>
            <div>
              <div className=testDescriptionText>{ReasonReact.string("Description: ")}</div>
              <Editable
                key={test.data.id ++ "_description"}
                editing={test.local.editingDescription}
                editingToggle={() => updateTest(Test.Action.ToggleEditDescription)}
                value={test.data.description}
                placeholder="Write description here"
                onTextChange={
                  text =>
                    updateTest(
                      Test.Action.ApolloUpdateTest(
                        () =>
                          mutation(
                            ~id=test.data.id,
                            ~name=test.data.name,
                            ~notes=test.data.notes,
                            ~description=text,
                          ),
                      ),
                    )
                }
                style=testDescriptionTextStyle
                useTextArea=true
              />
              <div className=testDescriptionText>{ReasonReact.string("Notes: ")}</div>
              <Editable
                key={test.data.id ++ "_notes"}
                editing={test.local.editingNotes}
                editingToggle={() => updateTest(Test.Action.ToggleEditNotes)}
                value={test.data.notes |> Belt.Option.getWithDefault(_, "")}
                placeholder="Write Some Notes Here"
                onTextChange={
                  text =>
                    updateTest(
                      Test.Action.ApolloUpdateTest(
                        () =>
                          mutation(
                            ~id=test.data.id,
                            ~name=test.data.name,
                            ~notes=Some(text),
                            ~description=test.data.description,
                          ),
                      ),
                    )
                }
                style=testDescriptionTextStyle
                useTextArea=true
              />
            </div>
         }
    </Test.Mutation.UpdateTest>,
};