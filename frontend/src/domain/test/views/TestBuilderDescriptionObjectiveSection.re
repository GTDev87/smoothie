let component =
  ReasonReact.statelessComponent("TestBuilderDescriptionObjectiveSection");

let css = Css.css;
let tw = Css.tw;

let builderDescriptionSectionClass = [%bs.raw
  {| css(tw` ml-4 w-full text-grey`)|}
];

let fullWidthStyle = ReactDOMRe.Style.make(~width="100%", ());

let objectiveInputStyle =
  ReactDOMRe.Style.make(~width="100%", ~marginBottom="1em", ());

let objectiveTitleStyle =
  ReactDOMRe.Style.make(
    ~borderBottom="solid 1px grey",
    ~paddingBottom=".2em",
    ~marginBottom="1em",
    ~textAlign="center",
    (),
  );

let objectiveStyle =
  ReactDOMRe.Style.make(~marginBottom=".2em", ~textAlign="center", ());

let make =
    (
      ~test: Test.Model.Record.t,
      ~updateTest,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self =>
    <Test.Mutation.AddObjective>
      ...{
           (~mutation as testAddObjectiveMutation) =>
             <Objective.Mutation.CreateUpdateObjective>
               ...{
                    (~mutation as createUpdateObjective) => {
                      let keydownAddObjective =
                        GenericInput.enterSubmitWithCommand(
                          () =>
                            updateTest(
                              Test.Action.ApolloAddObjective(
                                () =>
                                  testAddObjectiveMutation(
                                    ~testId=test.data.id,
                                    ~objectiveId=
                                      Objective.Model.getUUIDFromId(
                                        test.local.newObjectiveId,
                                      ),
                                  ),
                              ),
                            ),
                          () => updateTest(Test.Action.NoOpKeyDown),
                        );

                      let updateNewObjective = action =>
                        MyNormalizr.Converter.Objective.Remote.updateWithDefault(
                          (),
                          normalized |> Js.Promise.resolve,
                          test.local.newObjectiveId,
                          action,
                        )
                        |> updateNormalizr;

                      let valueEditNewObjectveText = text =>
                        updateNewObjective(
                          Objective.Action.ApolloEditText(
                            () =>
                              createUpdateObjective(
                                ~id=
                                  Objective.Model.getUUIDFromId(
                                    test.local.newObjectiveId,
                                  ),
                                ~text,
                              ),
                          ),
                        );

                      <div
                        key={test.local.newObjectiveId |> Objective.Model.getUUIDFromId}
                        className=builderDescriptionSectionClass>
                        <TextInput
                          key={test.local.newObjectiveId |> Objective.Model.getUUIDFromId}
                          style=objectiveInputStyle
                          onKeyDown=keydownAddObjective
                          placeholder="What is this test assessing?"
                          onTextChange=valueEditNewObjectveText
                          autoFocus=false
                          value={
                            normalized
                            |> MyNormalizr.Converter.Objective.Remote.getRecord(
                                 _,
                                 test.local.newObjectiveId,
                               )
                            |> Belt.Option.mapWithDefault(_, "", objective =>
                                 objective.data.text
                               )
                          }
                        />
                        <div style=fullWidthStyle>
                          <h3 style=objectiveTitleStyle>
                            {ReasonReact.string("Objectives")}
                          </h3>
                          {
                            test.data.objectiveIds
                            |> Utils.List.removeOptionsFromList
                            |> Belt.List.map(_, objectiveId =>
                                 <div
                                   key={objectiveId |> Objective.Model.getUUIDFromId}
                                   style=objectiveStyle>
                                   {
                                     normalized
                                     |> MyNormalizr.Converter.Objective.Remote.getRecord(
                                          _,
                                          objectiveId,
                                        )
                                     |> Belt.Option.mapWithDefault(
                                          _, "default", objective =>
                                          objective.data.text
                                        )
                                     |> ReasonReact.string
                                   }
                                 </div>
                               )
                            |> Utils.ReasonReact.listToReactArray
                          }
                        </div>
                      </div>;
                    }
                  }
             </Objective.Mutation.CreateUpdateObjective>
         }
    </Test.Mutation.AddObjective>,
};