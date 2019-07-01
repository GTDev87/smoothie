let component = ReasonReact.statelessComponent("StudentQuestionSidebar");
let css = Css.css;
let tw = Css.tw;
let studentQuestionLayoutQuestionClass = [%bs.raw {| css(tw` w-full mb-4`)|}];

let make =
    (
      ~data as studentQuestion: StudentQuestion.Model.Record.t,
      ~normalized,
      ~question,
      ~updateNormalizr,
      ~updateNormalizedWithReducer,
      _children,
    ) => {
  ...component,
  render: _self => {
    <StudentQuestion.Mutation.GiveAnswerKeyToQuestion>
      ...((~mutation as giveAnswerKeyToQuestionMutation) => {
        let studentAnswerKeys =
          StudentQuestion_Function.getStudentAnswerKeys(
            normalized,
            question,
            studentQuestion,
          );

        let studentQuestionAddKeyAnswer =
          StudentQuestion_Function.createStudentQuestionAddKeyAnswer(
            question, studentQuestion, giveAnswerKeyToQuestionMutation);

        <InfoCard header=(<StudentQuestionObjectiveHeader />)>
          {
            studentAnswerKeys
            |> Belt.List.map(_, studentAnswerKey =>
                <QuestionAnswerKeyLayout
                  key={studentAnswerKey.data.id}
                  normalized
                  updateNormalizr
                  updateNormalizedWithReducer={
                    norm =>
                      norm
                      |> updateNormalizedWithReducer
                      |> studentQuestionAddKeyAnswer(studentAnswerKey)
                  }
                  studentAnswerKey
                />
            )
            |> Utils.ReasonReact.listToReactArray
          }
        </InfoCard>;
      })
    </StudentQuestion.Mutation.GiveAnswerKeyToQuestion>
  }
};