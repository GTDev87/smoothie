let component =
  ReasonReact.statelessComponent("QuestionBuilderObjectiveSection");

let css = Css.css;
let tw = Css.tw;

let questionBuilderObjectiveAddButtonClass = [%bs.raw {| css(tw`
  w-full
  flex
  justify-center
  py-2
`)|}];

let questionBuilderObjectiveRowClass = [%bs.raw {| css(tw`
  w-full
  flex
  justify-around
`)|}];

let questionBuilderObjectiveRowCenterTextClass = [%bs.raw {| css(tw`
  w-full
  flex
  justify-center
`)|}];

let make =
    (
      ~test: Test.Model.Record.t,
      ~questionId: Question.Model.idType,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self => {
    let question =
      normalized
      |> MyNormalizr.getQuestionFromSchema(_, questionId)
      |> Belt.Option.getWithDefault(
           _, Question.Model.Record.defaultWithId(questionId));
    let questionBaseId = Question_Utils.questionToBaseId(question);
    
    let questionBase =
      normalized
      |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
      |> Belt.Option.getWithDefault(_, QuestionBase.Model.Record.defaultWithId((), questionBaseId));

    let updateQuestion = action =>
      MyNormalizr.Converter.QuestionBase.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        Schema.QuestionBase.stringToId(questionBase.data.id),
        action,
      )
      |> updateNormalizr;

    let updateAnswerKeyId = (id, action) =>
      MyNormalizr.Converter.QuestionAnswerKey.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        Schema.QuestionAnswerKey.stringToId(id),
        action,
      )
      |> updateNormalizr;

    let onKeyDownScore = id =>
      GenericInput.enterSubmitWithCommand(
        () => updateAnswerKeyId(id, QuestionAnswerKey.Action.ToggleEditScore),
        () => updateAnswerKeyId(id, QuestionAnswerKey.Action.NoOp),
      );
      
      <QuestionAnswerKey.Mutation.UpdateQuestionAnswerKey>
        ...{(~mutation) =>
          <InfoCard header=(
            <div>
              <div className=questionBuilderObjectiveRowCenterTextClass>
                {ReasonReact.string("Objectives")}
              </div>
              <div className=questionBuilderObjectiveRowClass>
                <div className=questionBuilderObjectiveRowCenterTextClass>
                  {ReasonReact.string("Name")}
                </div>
                <div className=questionBuilderObjectiveRowCenterTextClass>
                  {ReasonReact.string("Points")}
                </div>
              </div>
            </div>
          )>
            {
              Belt.List.map(
                questionBase.data.answerKeyIds,
                answerKeyId => {
                  let answerKey =
                    normalized
                    |> MyNormalizr.Converter.QuestionAnswerKey.Remote.getRecord(_, answerKeyId)
                    |> Belt.Option.getWithDefault(
                        _, QuestionAnswerKey.Model.Record.defaultWithId((), answerKeyId));
                  <div
                    className=questionBuilderObjectiveRowClass
                    key={QuestionAnswerKey.Model.getUUIDFromId(answerKeyId)}
                  >
                    <div className=questionBuilderObjectiveRowCenterTextClass>
                      <Select
                        key={answerKeyId |> QuestionAnswerKey.Model.getUUIDFromId}
                        selectedId={answerKey.data.objectiveId}
                        idToString={
                          objectiveIdMaybe =>
                            switch (objectiveIdMaybe) {
                            | None => "None"
                            | Some(objectiveId) =>
                              Objective.Model.getUUIDFromId(objectiveId)
                            }
                        }
                        selections={
                          (
                            test.data.objectiveIds
                            |> Belt.List.map(_, (objectiveIdMaybe: option(Objective.Model.idType)) =>
                                ({
                                  id: objectiveIdMaybe,
                                  text:
                                    Belt.Option.mapWithDefault(objectiveIdMaybe, "None", objectiveId =>
                                      normalized
                                      |> MyNormalizr.Converter.Objective.Remote.getRecord(_, objectiveId)
                                      |> Belt.Option.mapWithDefault(_, "None", objective => objective.data.text)
                                    ),
                                }: Select.selectionType(option(Objective.Model.idType)))
                              ) 
                          ) @ [
                            {
                              id: None,
                              text: "None",
                            }: Select.selectionType(option(Objective.Model.idType))
                          ]
                        }
                        onSelect={
                          a =>
                            updateAnswerKeyId(
                              answerKey.data.id,
                              QuestionAnswerKey.Action.ApolloUpdateQuestionAnswer(
                                () =>
                                  mutation(
                                    ~id=answerKey.data.id,
                                    ~score=answerKey.data.score,
                                    ~objectiveId=
                                      Belt.Option.getWithDefault(a, None),
                                    (),
                                  ),
                              ),
                            )
                        }
                      />
                    </div>
                    <div className=questionBuilderObjectiveRowCenterTextClass>
                      <Editable
                        /* TODO need to give it an intermediat score thing */
                        editing={answerKey.local.editingScore}
                        editingToggle={
                          () =>
                            updateAnswerKeyId(
                              answerKey.data.id,
                              QuestionAnswerKey.Action.ToggleEditScore,
                            )
                        }
                        value={answerKey.data.score |> string_of_float}
                        placeholder="1."
                        onTextChange={
                          scoreText =>
                            updateAnswerKeyId(
                              answerKey.data.id,
                              QuestionAnswerKey.Action.ApolloUpdateQuestionAnswer(
                                () =>
                                  mutation(
                                    ~id=answerKey.data.id,
                                    ~score=float_of_string(scoreText),
                                    (),
                                  ),
                              ),
                            )
                        }
                        onKeyDown={onKeyDownScore(answerKey.data.id)}
                      />
                    </div>
                  </div>;
                },
              )
              |> Utils.ReasonReact.listToReactArray
            }
            <Question.Mutation.AddAnswerKey>
              ...{(~mutation) =>
                <div className=questionBuilderObjectiveAddButtonClass>
                  <Button
                    onClick={
                      _a =>
                        updateQuestion(
                          QuestionBase.Action.ApolloAddAnswerKey(
                            () =>
                              mutation(
                                ~id=
                                  QuestionAnswerKey.Model.getUUIDFromId(
                                    questionBase.local.newAnswerKeyId,
                                  ),
                                ~questionId=questionBase.data.id,
                              ),
                          ),
                        )
                        |> ignore
                    }>
                    {ReasonReact.string("Add Objective")}
                  </Button>
                </div>
              }
            </Question.Mutation.AddAnswerKey>
          </InfoCard>
        }
      </QuestionAnswerKey.Mutation.UpdateQuestionAnswerKey>
  },
};