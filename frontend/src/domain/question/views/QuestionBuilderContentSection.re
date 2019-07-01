let component =
  ReasonReact.statelessComponent("QuestionBuilderContentSection");

let addOne = a => a + 1;

let fullSizeSpaceBetween =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~flexDirection="column",
    ~justifyContent="space-between",
    ~width="100%",
    ~height="100%",
    ~padding="0",
    ~margin="0",
    (),
  );

let questionTextStyle =
  ReactDOMRe.Style.make(
    ~paddingBottom="3em",
    ~paddingTop=".2em",
    ~backgroundColor="inherit",
    ~height="100%",
    ~width="100%",
    ~wordWrap="break-word",
    ~wordBreak="break-all",
    (),
  );

let fullWidthStyle =
  ReactDOMRe.Style.make(~width="100%", ~marginBottom="1em", ());

let questionSectionStyle =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~marginBottom="1em",
    ~padding=".3em",
    ~border="solid grey 1px",
    (),
  );

let answerSectionStyle =
  ReactDOMRe.Style.make(
    ~width="100%",
    ~marginBottom="1em",
    ~padding=".3em",
    ~display="flex",
    ~border="solid grey 1px",
    (),
  );

let answerText = ReactDOMRe.Style.make(~marginRight="2em", ());

let fullSize =
  ReactDOMRe.Style.make(~height="100%", ~width="100%", ~margin="0", ());

let stimulusStyle =
  ReactDOMRe.Style.make(~width="100%", ~marginBottom="1em", ());

let numberStyle = ReactDOMRe.Style.make(~color="grey", ());

let questionTypeStyle =
  ReactDOMRe.Style.make(~width="100%", ~padding="0", ~margin="0", ());

let make =
    (
      ~questionId: Schema.Question.id,
      ~number,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~updateQuestionType,
      _children,
    ) => {
  ...component,
  render: _self => {
    let question =
      normalized
      |> MyNormalizr.getQuestionFromSchema(_, questionId)
      |> Belt.Option.getWithDefault(
           _,
           Question.Model.Record.defaultWithId(questionId),
         );
    let questionBaseId = Question_Utils.questionToBaseId(question);

    let questionBase =
      normalized
      |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(
           _, questionBaseId)
      |> Belt.Option.getWithDefault(
           _, QuestionBase.Model.Record.defaultWithId((), questionBaseId));

    let updateQuestionBase = action =>
      MyNormalizr.Converter.QuestionBase.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        questionBaseId,
        action,
      )
      |> updateNormalizr;

    let onKeyDownSolution =
      GenericInput.enterSubmitWithCommand(
        () => updateQuestionBase(QuestionBase.Action.ToggleEditSolution),
        () => updateQuestionBase(QuestionBase.Action.NoOp),
      );

    <UpdateQuestion>
      ...{
           (~mutation as updateQuestionMutation) =>
             <div style=fullSizeSpaceBetween>
               <div style=fullWidthStyle>
                 <div style=stimulusStyle>
                   {
                     switch (questionBase.data.stimulusId) {
                     | None =>
                       <div style=fullWidthStyle>
                         <AddStimulus>
                           ...(
                                (~mutation as mutationAddStimulus) =>
                                  <button
                                    onClick={
                                      _e =>
                                        updateQuestionBase(
                                          QuestionBase.Action.ApolloCreateStimulus(
                                            () =>
                                              mutationAddStimulus(
                                                ~id=UUID.generateUUID(),
                                                ~questionId=
                                                  Schema.getQuestionUUIDFromId(
                                                    questionId,
                                                  ),
                                              ),
                                          ),
                                        )
                                        |> ignore
                                    }>
                                    {ReasonReact.string("Add stimulus")}
                                  </button>
                              )
                         </AddStimulus>
                       </div>
                     | Some(stimulusId) =>
                       let updateStimulus = action =>
                         MyNormalizr.Converter.Stimulus.Remote.updateWithDefault(
                           (),
                           normalized |> Js.Promise.resolve,
                           stimulusId,
                           action,
                         )
                         |> updateNormalizr;

                       <div
                         style=fullWidthStyle
                         key={stimulusId |> Stimulus.Model.getUUIDFromId}>
                         <StimulusLayout
                           stimulusId
                           normalized /* heres */
                           editable=true
                           updateStimulus=(a => updateStimulus(a))
                         />
                       </div>;
                     }
                   }
                 </div>
                 <div style=questionSectionStyle>
                   <Row style=fullSize>
                     <Col xs=12>
                       <Editable
                         key={questionBase.data.id}
                         editing={questionBase.local.editingText}
                         editingToggle={
                           () =>
                             updateQuestionBase(
                               QuestionBase.Action.ToggleEditText,
                             )
                         }
                         value={questionBase.data.text}
                         placeholder="Question Goes Here."
                         onTextChange={
                           text =>
                             updateQuestionBase(
                               QuestionBase.Action.ApolloUpdateQuestion(
                                 () =>
                                   updateQuestionMutation(
                                     ~id=questionBase.data.id,
                                     ~text,
                                     ~solution=questionBase.data.solution,
                                     ~autoScore=questionBase.data.autoScore,
                                     ~forcedResponse=
                                       questionBase.data.forcedResponse,
                                   ),
                               ),
                             )
                         }
                         useTextArea=true
                         style=questionTextStyle
                       />
                     </Col>
                   </Row>
                 </div>
                 <div style=answerSectionStyle>
                   <h3 style=answerText className="h3">
                     {ReasonReact.string("Answer:")}
                   </h3>
                   <MultipleChoiceQuestion.Mutation.AddChoice>
                     ...{
                      (~mutation as mutationAddChoice) => {
                        switch (question) {
                        | MultipleChoiceQuestion(multipleChoiceQuestion) =>
                          let multipleChoiceId =
                            multipleChoiceQuestion.data.multipleChoiceId;

                          let updateMultipleChoice = action =>
                            MyNormalizr.Converter.MultipleChoice.Remote.updateWithDefault(
                              (),
                              normalized |> Js.Promise.resolve,
                              multipleChoiceId,
                              action,
                            )
                            |> updateNormalizr;

                          /* if multiple choice is not set then try and set it*/

                          let updateMultipleChoiceQuestion = action =>
                            MyNormalizr.Converter.MultipleChoiceQuestion.Remote.updateWithDefault(
                              (),
                              normalized |> Js.Promise.resolve,
                              Schema.MultipleChoiceQuestion.stringToId(
                                multipleChoiceQuestion.data.id,
                              ),
                              action,
                            )
                            |> MyNormalizr.Converter.MultipleChoice.Remote.updateWithDefault(
                                  (),
                                  _,
                                  multipleChoiceId,
                                  MultipleChoice.Action.ResetOption,
                                )
                            |> updateNormalizr;

                          <MultipleChoiceCreator
                            updateMultipleChoice
                            multipleChoiceId
                            updateAddChoice=(
                              (~id, ~text) =>
                                updateMultipleChoiceQuestion(
                                  MultipleChoiceQuestion.Action.ApolloAddChoice(
                                    () =>
                                      mutationAddChoice(
                                        ~id,
                                        ~questionId=
                                          multipleChoiceQuestion.data.id,
                                        ~text,
                                      ),
                                  ),
                                )
                            )
                            normalized
                            onSelect=(
                              selection =>
                                updateQuestionBase(
                                  QuestionBase.Action.ApolloUpdateQuestion(
                                    () =>
                                      updateQuestionMutation(
                                        ~id=questionBase.data.id,
                                        ~text=questionBase.data.text,
                                        ~solution=selection,
                                        ~autoScore=
                                          questionBase.data.autoScore,
                                        ~forcedResponse=
                                          questionBase.data.forcedResponse,
                                      ),
                                  ),
                                )
                            )
                            selectedId={questionBase.data.solution}
                          />
                          | TrueFalseQuestion(trueFalseQuestion) =>
                            <MultipleChoiceSelector
                              selectedId=questionBase.data.solution
                              selections=[
                                {id: "true", text: "true"},
                                {id: "false", text: "false"},
                              ]
                              onSelect=(
                                selection =>
                                updateQuestionBase(
                                  QuestionBase.Action.ApolloUpdateQuestion(
                                    () =>
                                      updateQuestionMutation(
                                        ~id=questionBase.data.id,
                                        ~text=questionBase.data.text,
                                        ~solution=selection,
                                        ~autoScore=
                                          questionBase.data.autoScore,
                                        ~forcedResponse=
                                          questionBase.data.forcedResponse,
                                      ),
                                  ),
                                )
                              )
                            />

                            | _ =>
                              <Editable
                                key={questionBase.data.id ++ "_answer"}
                                editing={questionBase.local.editingSolution}
                                editingToggle=(
                                  () =>
                                    updateQuestionBase(
                                      QuestionBase.Action.ToggleEditSolution,
                                    )
                                )
                                value={questionBase.data.solution}
                                placeholder="Answer Goes here."
                                onTextChange=(
                                  text =>
                                    updateQuestionBase(
                                      QuestionBase.Action.ApolloUpdateQuestion(
                                        () =>
                                          updateQuestionMutation(
                                            ~id=questionBase.data.id,
                                            ~text=questionBase.data.text,
                                            ~solution=text,
                                            ~autoScore=
                                              questionBase.data.autoScore,
                                            ~forcedResponse=
                                              questionBase.data.forcedResponse,
                                          ),
                                      ),
                                    )
                                )
                                onKeyDown=onKeyDownSolution
                                useTextArea=true
                                style=questionTextStyle
                              />
                            }
                          }
                        }
                   </MultipleChoiceQuestion.Mutation.AddChoice>
                   {
                    switch (question) {
                    | FillInTheBlankQuestion(fillInTheBlankQuestion) =>
                        <div>
                          {ReasonReact.string("On Fill In the blank questions add ______ where the student should fill in their answer.")}
                        </div>
                    | _ => <div />
                    }
                   }
                 </div>
                 <div style=fullWidthStyle>
                   <LabelContent
                     labelContentList=[
                       (
                         "Auto Score",
                         <Checkbox
                           id="Auto Score"
                           key={questionBase.data.id ++ "_autoScore"}
                           value={questionBase.data.autoScore}
                           onToggle={
                             () =>
                               updateQuestionBase(
                                 QuestionBase.Action.ApolloUpdateQuestion(
                                   () =>
                                     updateQuestionMutation(
                                       ~id=questionBase.data.id,
                                       ~text=questionBase.data.text,
                                       ~solution=questionBase.data.solution,
                                       ~autoScore=!questionBase.data.autoScore,
                                       ~forcedResponse=
                                         questionBase.data.forcedResponse,
                                     ),
                                 ),
                               )
                           }
                         />,
                       ),
                       (
                         "Force Response",
                         <Checkbox
                           key={questionBase.data.id ++ "_forcedResponse"}
                           id="Force Response"
                           value={questionBase.data.forcedResponse}
                           onToggle={
                             () =>
                               updateQuestionBase(
                                 QuestionBase.Action.ApolloUpdateQuestion(
                                   () =>
                                     updateQuestionMutation(
                                       ~id=questionBase.data.id,
                                       ~text=questionBase.data.text,
                                       ~solution=questionBase.data.solution,
                                       ~autoScore=questionBase.data.autoScore,
                                       ~forcedResponse=
                                         !questionBase.data.forcedResponse,
                                     ),
                                 ),
                               )
                           }
                         />,
                       ),
                     ]
                   />
                 </div>
               </div>
               <div style=questionTypeStyle>
                 <QuestionTypeSelect
                   questionId
                   normalized
                   updateNormalizr
                   updateQuestionType
                 />
               </div>
             </div>
         }
    </UpdateQuestion>;
  },
};