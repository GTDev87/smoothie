let component = ReasonReact.statelessComponent("TestWriter");

let testDescriptionTextStyle =
  ReactDOMRe.Style.make(
    ~backgroundColor="inherit",
    ~width="100%",
    ~wordWrap="break-word",
    ~wordBreak="break-all",
    (),
  );

let testTitleH1Style = ReactDOMRe.Style.make(~marginTop="0", ());

let fullWidth = ReactDOMRe.Style.make(~width="100%", ());

let fullSize = ReactDOMRe.Style.make(~width="100%", ~height="100%", ());

let contentSectionStyle =
  ReactDOMRe.Style.make(
    ~height="100%",
    ~width="100%",
    ~display="flex",
    ~flexDirection="column",
    ~paddingRight="1em",
    (),
  );

let css = Css.css;
let tw = Css.tw;

let paginationClass = [%bs.raw {| css(tw` w-full flex justify-between`)|}];

let make =
    (
      ~studentTest: StudentTest.Model.Record.t,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~updateNormalizedWithReducer,
      _children,
    ) => {
  ...component,
  render: _self => {
    let testId = studentTest.data.originalId;
    let updateTest = action =>
      MyNormalizr.Converter.Test.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        testId,
        action,
      )
      |> updateNormalizr;

    switch (MyNormalizr.Converter.Test.Remote.getRecord(normalized, testId)) {
    | None => <div />
    | Some(test) =>
      <TestLayout
        footer={
          <div className=paginationClass>
            <Paginator
              selectedId={test.local.selectedQuestion}
              onSelect=(
                qid =>
                  updateTest(Test.Action.ChangeSelectedQuestion(qid))
                  |> ignore
              )
              selections={
                Belt.List.mapWithIndex(
                  test.data.questionIds, (idx, questionId) =>
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
            <PaginatorButton
              selection=(
                {id: None, text: "Overview"}:
                  PaginatorType.selectionType(option(Schema.Question.id)))
              isSelected={None == test.local.selectedQuestion}
              onClick=(
                _ =>
                  updateTest(Test.Action.ChangeSelectedQuestion(None))
                  |> ignore
              )
            />
          </div>
        }>
        <div style=contentSectionStyle>
          {
            switch (test.local.selectedQuestion) {
            | None =>
              <div style=fullWidth>
                <div style=fullWidth>
                  <h4> {ReasonReact.string("Description: ")} </h4>
                  <h4> {ReasonReact.string(test.data.description)} </h4>
                </div>
                <div style=fullWidth>
                  <b> {ReasonReact.string("Objectives: ")} </b>
                  {
                    test.data.objectiveIds
                    |> Utils.List.removeOptionsFromList
                    |> MyNormalizr.Converter.Objective.idListToFilteredItems(
                         _,
                         MyNormalizr.Converter.Objective.Remote.getRecord(
                           normalized,
                         ),
                       )
                    |> Belt.List.map(_, objective => objective.data.text)
                    |> Utils.List.joinStringList(_, ", ")
                    |> ReasonReact.string
                  }
                </div>
              </div>
            | Some(selectedQuestionId) =>
              test.data.questionIds
              |> Belt.List.mapWithIndex(_, (idx, questionId) =>
                   (idx, questionId)
                 )
              |> Belt.List.getBy(_, ((_, questionId)) =>
                   questionId
                   |> Schema.getQuestionUUIDFromId == selectedQuestionId
                 )
              |> Belt.Option.mapWithDefault(_, <div />, ((idx, questionId)) =>
                   studentTest.data.questionIds
                   |> MyNormalizr.Converter.StudentQuestion.idListToFilteredItems(
                        _, MyNormalizr.Converter.StudentQuestion.Remote.getRecord(normalized))
                   |> Belt.List.getBy(_, studentQuestion => studentQuestion.data.originalId == questionId)
                   |> Belt.Option.getWithDefault(_, StudentQuestion.Model.Record.default(questionId))
                   |> (
                     studentQuestion =>
                       <div style=fullSize key={studentQuestion.data.id}>
                         <StudentTest.Mutation.GiveQuestionToTest>
                           ...{
                                (~mutation) =>
                                  <StudentQuestionLayout
                                    number={idx + 1}
                                    normalized
                                    updateNormalizr
                                    studentQuestion
                                    testNotes=test.data.notes
                                    updateNormalizedWithReducer={
                                      Student_Function.createStudentQuestionTestNormalizr(
                                        updateNormalizedWithReducer,
                                        test,
                                        studentTest,
                                        mutation,
                                        studentQuestion,
                                      )
                                    }
                                  />
                              }
                         </StudentTest.Mutation.GiveQuestionToTest>
                       </div>
                   )
                 )
            }
          }
        </div>
      </TestLayout>
    };
  },
};