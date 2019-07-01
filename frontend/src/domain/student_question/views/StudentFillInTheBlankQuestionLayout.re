let component = ReasonReact.statelessComponent("StudentFillInTheBlankQuestionLayout");

let css = Css.css;
let tw = Css.tw;


let fillInTheBlankInputClass = [%bs.raw {| css(tw`
  inline-flex
  w-12
  mx-4
`)|}];
let inputClass = [%bs.raw {| css(tw`
  border-0
  border-b
  border-grey
  border-solid
`)|}];

let splitByUnderscores = (text) : list(string) =>
  [%re "/\\s*\\_+\\s*/"]
  |> Js.String.splitByReAtMost(_, ~limit=2, text)
  |> Belt.List.fromArray;

let studentQuestionLayoutAnswerClass = [%bs.raw {| css(tw` w-full mb-1`)|}];
let make =
    (
      ~data as studentQuestion: StudentQuestion.Model.Record.t,
      ~questionBase: QuestionBase.Model.Record.t,
      ~updateStudentAnswer,
      ~blankPlaceholder=false,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div>
      {
        questionBase.data.text
        |> splitByUnderscores
        |> (listOfStrings) => {
          switch(listOfStrings){
          | [firstHalf, secondHalf] =>
            <div>
              {firstHalf |> Utils.String.stringToDivHtml}
              <div className=fillInTheBlankInputClass>
                <TextInput
                  className=inputClass
                  placeholder=(blankPlaceholder ? "" : "Fill in the blank here")
                  value={studentQuestion.data.answer}
                  onTextChange=updateStudentAnswer
                  autoFocus=false
                />
              </div>
              {secondHalf |> Utils.String.stringToDivHtml}
            </div>
          | _ => <div>{ReasonReact.string("Fill In the blank is not complete")}</div>
          }
        }
      }
    </div>
};