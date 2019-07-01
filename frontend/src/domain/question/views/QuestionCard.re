let component = ReasonReact.statelessComponent("QuestionCard");
let css = Css.css;
let tw = Css.tw;

let fullWidthClass = [%bs.raw {| css(tw` w-full`)|}];
let studentQuestionLayoutQuestionClass = [%bs.raw {| css(tw` w-full mb-4`)|}];
let studentQuestionLayoutAnswerClass = [%bs.raw {| css(tw` w-full mb-1`)|}];

let questionCardHeaderClass = [%bs.raw
  {| css(tw`
    pl-2
    pt-1
  `)|}
];

let questionCardContentClass = [%bs.raw
  {| css(tw`
    px-2
    pt-2
  `)|}
];
let emptySpaceClass = [%bs.raw {| css(tw` h-64 `)|}];

let questionTextClass = [%bs.raw {| css(tw` w-full font-semibold`)|}];

let make =
    (
      ~normalized,
      ~question: Question.Model.Record.t,
      ~number,
      ~teacherView=false,
      ~blankPlaceholder=false,
      ~updateNormalizr:
         option(
           Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
         )=?,
      ~updateNormalizedWithReducer=?,
      ~studentQuestion: option(StudentQuestion.Model.Record.t)=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    let updateNormalizr =
      Belt.Option.getWithDefault(updateNormalizr, _ => Js.Promise.resolve());

    let updateNormalizedWithReducer =
      Belt.Option.getWithDefault(updateNormalizedWithReducer, _ =>
        Js.Promise.resolve(normalized)
      );

    let questionBaseId = Question_Utils.questionToBaseId(question);

    let studentQuestionVal =
      Belt.Option.getWithDefault(
        studentQuestion,
        StudentQuestion.Model.Record.default(
          Schema.Question.LongAnswer(
            questionBaseId |> QuestionBase.Model.getUUIDFromId,
          ),
        ),
      );

    let studentAnswerKeys =
      StudentQuestion_Function.getStudentAnswerKeys(
        normalized,
        question,
        studentQuestionVal,
      );

    let questionBase =
      normalized
      |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
      |> Belt.Option.getWithDefault(_, QuestionBase.Model.Record.defaultWithId((), questionBaseId));

    let answer = teacherView ? Some(questionBase.data.solution) : None;

    <StudentQuestion.Mutation.GiveAnswerKeyToQuestion>
      ...{(~mutation as giveAnswerKeyToQuestionMutation) => {
        <StudentQuestion.Mutation.UpdateStudentQuestion>
          ...{(~mutation as updateStudentQuestionMutation) => {
            let studentQuestionAddKeyAnswer =
              StudentQuestion_Function.createStudentQuestionAddKeyAnswer(
                question,
                studentQuestionVal,
                giveAnswerKeyToQuestionMutation,
              );
            <StudentAnswerKey.Mutation.UpdateStudentAnswerKey>
              ...{(~mutation as updateStudentAnswerKey) => {
                let updateStudentAnswer =
                  StudentQuestion_Function.createUpdateStudentAnswer(
                    normalized,
                    updateNormalizedWithReducer,
                    updateNormalizr,
                    question,
                    studentQuestionVal,
                    studentAnswerKeys,
                    studentQuestionAddKeyAnswer,
                    updateStudentQuestionMutation,
                    updateStudentAnswerKey,
                  );

                <InfoCard
                  header=(
                    <h3 className=questionCardHeaderClass>
                      {ReasonReact.string("Question " ++ (number |> string_of_int))}
                    </h3>
                  )
                >
                  <div className=questionCardContentClass>
                    {
                      Belt.Option.mapWithDefault(
                        questionBase.data.stimulusId,
                        <div key="none" />,
                        stimulusId => {
                          let updateStimulus = action =>
                            MyNormalizr.Converter.Stimulus.Remote.updateWithDefault(
                              (),
                              normalized |> Js.Promise.resolve,
                              stimulusId,
                              action,
                            )
                            |> updateNormalizr;
                          <div
                            className=studentQuestionLayoutQuestionClass
                            key={
                              stimulusId
                              |> Stimulus.Model.getUUIDFromId
                            }>
                            <StimulusLayout
                              stimulusId
                              normalized /* heres */
                              editable=false
                              updateStimulus={
                                a => updateStimulus(a)
                              }
                            />
                          </div>;
                        },
                      )
                    }
                    <div
                      className=studentQuestionLayoutQuestionClass>
                      <div className=questionTextClass>
                        {
                          switch (question) {
                          | FillInTheBlankQuestion(fillInTheBlankQuestion) =>
                            <StudentFillInTheBlankQuestionLayout
                              data=studentQuestionVal
                              questionBase
                              updateStudentAnswer
                              blankPlaceholder
                            />
                          | _ => questionBase.data.text |> Utils.String.stringToDivHtml
                          }
                        }
                      </div>
                    </div>
                    {
                      switch (question) {
                      | MultipleChoiceQuestion(multipleChoiceQuestion) =>
                        let multipleChoiceId =
                          multipleChoiceQuestion.data.multipleChoiceId;

                        <MultipleChoiceLayout
                          multipleChoiceId
                          normalized
                          answer
                          onSelect=updateStudentAnswer
                          selectedId={studentQuestionVal.data.answer}
                        />;
                      | TrueFalseQuestion(trueFalseQuestion) =>
                          let multipleChoiceId =
                            trueFalseQuestion.data.multipleChoiceId;

                          <MultipleChoiceLayout
                            multipleChoiceId
                            normalized
                            answer
                            onSelect=updateStudentAnswer
                            selectedId={studentQuestionVal.data.answer}
                          />;
                      | LongAnswerQuestion(_) =>
                        <StudentLongAnswerQuestionAnswerLayout
                          data=studentQuestionVal
                          updateStudentAnswer
                          blankPlaceholder
                        />
                      | FillInTheBlankQuestion(_) => <div/>
                      }
                    }
                  </div>
                </InfoCard>
              }}
            </StudentAnswerKey.Mutation.UpdateStudentAnswerKey>;
          }}
        </StudentQuestion.Mutation.UpdateStudentQuestion>
      }}
    </StudentQuestion.Mutation.GiveAnswerKeyToQuestion>;
  }
};