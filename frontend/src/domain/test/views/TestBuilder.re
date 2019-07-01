let component = ReasonReact.statelessComponent("TestBuilder");

let questionButtonStyle =
  ReactDOMRe.Style.make(
    ~backgroundColor="#dadada",
    ~border="none",
    ~padding="0 .3em",
    ~margin="0 .1em",
    ~cursor="pointer",
    (),
  );

let contentSectionStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~width="100%",
    ~display="flex",
    ~flexDirection="column",
    (),
  );

let remainingStyle = ReactDOMRe.Style.make(~flex="2", ~overflow="auto", ());

let css = Css.css;
let tw = Css.tw;

let paginationClass = [%bs.raw {| css(tw` w-full flex`)|}];

let testHeaderClass = [%bs.raw {| css(tw` w-full flex`)|}];

let testHeaderTitleClass = [%bs.raw {| css(tw` w-2/3`)|}];
let testHeaderButtonClass = [%bs.raw {| css(tw` w-1/6 flex justify-center`)|}];

let make =
    (
      ~testId,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self =>
    switch (MyNormalizr.Converter.Test.Remote.getRecord(normalized, testId)) {
    | None => <div />
    | Some(test) =>
      let testId = Schema.Test.stringToId(test.data.id);
      let updateTest = action =>
        MyNormalizr.Converter.Test.Remote.updateWithDefault(
          (),
          normalized |> Js.Promise.resolve,
          testId,
          action,
        )
        |> updateNormalizr;

      let handleTestNameKeydown =
        GenericInput.enterSubmitWithCommand(
          () => updateTest(Test.Action.ToggleEditName),
          () => updateTest(Test.Action.NoOpKeyDown),
        );

      let selectionTuple =
        Belt.List.mapWithIndex(test.data.questionIds, (idx, questionId) =>
          (idx, questionId));

      /* TODO selected question is wrong*/
      /* need to select by index*/ 

      let (selectedQuestionNumber, selectedQuestion) =
        selectionTuple
        |> Belt.List.getBy(_, ((_, questionId)) => Some(questionId |> Schema.getQuestionUUIDFromId) == test.local.selectedQuestion)
        |> Belt.Option.mapWithDefault(_, (0, None), ((idx, q)) => (idx, Some(q)));

      let link = Document.origin ++ "/preview/" ++ test.data.id;

      <TestLayout
        header={
          <Test.Mutation.UpdateTest>
            ...((~mutation) =>
              <div className=testHeaderClass>
                <div className=testHeaderTitleClass>
                  <h1>
                    <Editable
                      editing={test.local.editingName}
                      editingToggle={() => updateTest(Test.Action.ToggleEditName)}
                      value={test.data.name}
                      placeholder="Name your test"
                      onTextChange={
                        text =>
                          updateTest(
                            Test.Action.ApolloUpdateTest(
                              () =>
                                mutation(
                                  ~id=test.data.id,
                                  ~name=text,
                                  ~notes=test.data.notes,
                                  ~description=test.data.description,
                                ),
                            ),
                          )
                      }
                      onKeyDown=handleTestNameKeydown
                    />
                  </h1>
                </div>
                <div className=testHeaderButtonClass>
                  <a href=link target="_blank">
                    <Button theme=CTA>
                      {ReasonReact.string("Preview")}
                    </Button>
                  </a>
                </div>
                <div className=testHeaderButtonClass>
                  <div>
                    <Button
                      theme=CTA
                      onClick=(_ => updateTest(Test.Action.ChangeSelectedQuestion(None))|> ignore)
                    >
                      {ReasonReact.string("Overview")}
                    </Button>
                  </div>
                </div>
              </div>
            )
          </Test.Mutation.UpdateTest>
        }
        footer={
          <div className=paginationClass>
            <div style=remainingStyle>
              <Paginator
                selectedId={test.local.selectedQuestion}
                onSelect=(
                  qid =>
                    updateTest(Test.Action.ChangeSelectedQuestion(qid))
                    |> ignore
                )
                selections={
                  Belt.List.map(selectionTuple, ((idx, questionId)) =>
                    (
                      {
                        id: Some(questionId |> Schema.getQuestionUUIDFromId),
                        text: idx |> (a => a + 1 |> string_of_int),
                      }:
                        PaginatorType.selectionType(option(UUID.t))
                    )
                  )
                }
              />
            </div>
            <div>
              <Test.Mutation.AddQuestion>
                ...(
                     (~mutation) =>
                       <div
                         style=questionButtonStyle
                         onClick={
                           _ =>
                             updateTest(
                               Test.Action.(
                                 CombineReducer(
                                   ApolloAddQuestion(
                                     () =>
                                       mutation(
                                         ~id=
                                           Schema.getQuestionUUIDFromId(
                                             test.local.newQuestionId,
                                           ),
                                         ~testId=test.data.id,
                                       ),
                                   ),
                                   ChangeSelectedQuestion(
                                     Some(
                                       test.local.newQuestionId
                                       |> Schema.getQuestionUUIDFromId,
                                     ),
                                   ),
                                 )
                               ),
                             )
                             |> ignore
                         }>
                         {ReasonReact.string("Add Question +")}
                       </div>
                   )
              </Test.Mutation.AddQuestion>
            </div>
          </div>
        }>
        {
          switch (selectedQuestion) {
          /* reference issue */
          | None =>
            <TestBuilderContentSection
              sidebar={
                <TestBuilderDescriptionObjectiveSection
                  test
                  updateTest
                  normalized
                  updateNormalizr
                />
              }>
              <TestBuilderDescriptionContentSection test updateTest />
            </TestBuilderContentSection>
          | Some(questionId) =>
            <TestBuilderContentSection
              sidebar={
                <QuestionBuilderObjectiveSection
                  test
                  questionId
                  normalized
                  updateNormalizr
                />
              }>
              <Test.Mutation.UpdateQuestionType>
                ...((~mutation) =>
                  <div style=contentSectionStyle>
                    <QuestionBuilderContentSection
                      questionId
                      normalized
                      updateNormalizr
                      number={selectedQuestionNumber + 1}
                      updateQuestionType={
                        (~id, ~questionType) =>
                          mutation(
                            ~testId=test.data.id,
                            ~questionType,
                            ~id,
                          )
                      }
                    />
                  </div>
                )
              </Test.Mutation.UpdateQuestionType>
            </TestBuilderContentSection>
          }
        }
      </TestLayout>;
    },
};