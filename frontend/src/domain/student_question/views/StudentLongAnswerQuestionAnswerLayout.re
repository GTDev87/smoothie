let component = ReasonReact.statelessComponent("StudentLongAnswerQuestionAnswerLayout");

let css = Css.css;
let tw = Css.tw;

let studentQuestionLayoutAnswerClass = [%bs.raw {| css(tw` w-full mb-1`)|}];
let make =
    (
      ~data as studentQuestion: StudentQuestion.Model.Record.t,
      ~updateStudentAnswer,
      ~blankPlaceholder=false,
      _children,
    ) => {
  ...component,
  render: _self =>
    <TextArea
      placeholder=(blankPlaceholder ? "" : "Fill in your answer")
      value={studentQuestion.data.answer}
      onTextChange=updateStudentAnswer
      autoFocus=false
    />
};