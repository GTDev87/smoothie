let component = ReasonReact.statelessComponent("StudentQuestionLayout");
let css = Css.css;
let tw = Css.tw;
let fullWidth = ReactDOMRe.Style.make(~width="100%", ());
let studentQuestionLayoutQuestionClass = [%bs.raw {| css(tw` w-full mb-4`)|}];
let questionCardContainerClass = [%bs.raw {| css(tw`
  flex
  flex-col
  h-full
  w-full
  overflow-y-hidden
`)|}];

let questionCardContainerMainClass = [%bs.raw {| css(tw`
  overflow-y-scroll
  flex-1
  relative
  w-full
`)|}];

let questionCardContainerMainInternalClass = [%bs.raw {| css(tw`
  absolute
  pin-t
  pin-l
  pin-b
  pin-r
  w-full
  h-full
`)|}];

let questionCardContainerFooterClass = [%bs.raw {| css(tw`
  w-full
  pt-2
  px-2
  flex-no-grow
  rounded
  border
  border-solid
  border-blue-light
`)|}];

let make =
    (
      ~studentQuestion: StudentQuestion.Model.Record.t,
      ~normalized,
      ~number,
      ~testNotes,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~updateNormalizedWithReducer,
      _children,
    ) => {
  ...component,
  render: _self =>
    switch (MyNormalizr.getQuestionFromSchema(normalized, studentQuestion.data.originalId)) {
    | None => <div />
    | Some(question) =>
      /* this is kind of confusing */
      let questionBaseId = Question_Utils.questionToBaseId(question);
      
      let questionBase =
        normalized
        |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
        |> Belt.Option.getWithDefault(_, QuestionBase.Model.Record.defaultWithId((), questionBaseId));

      <TestBuilderContentSection
        sidebar={
          <StudentQuestionSidebar
            data=studentQuestion
            normalized
            question
            updateNormalizr
            updateNormalizedWithReducer
          />
        }
      >
        <div className=questionCardContainerClass>
          <div className=questionCardContainerMainClass>
            <div className=questionCardContainerMainInternalClass>
              <QuestionCard
                normalized
                updateNormalizr
                number
                teacherView=true
                question
                studentQuestion
                updateNormalizedWithReducer
              />
            </div>
          </div>
          <div className=questionCardContainerFooterClass>
            <QuestionBaseTeacherDisplay data=questionBase testNotes />
          </div>
        </div>
      </TestBuilderContentSection>;
    },
};