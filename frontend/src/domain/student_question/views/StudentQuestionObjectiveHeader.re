let component = ReasonReact.statelessComponent("StudentQuestionObjectiveHeader");
let css = Css.css;
let tw = Css.tw;
let studentQuestionObjectiveHeader = [%bs.raw {| css(tw` py-2 w-full flex flex-wrap text-black`)|}];
let studentQuestionObjectiveHeaderTextWrapper = [%bs.raw {| css(tw` w-1/3 flex justify-center`)|}];

let make =(_children,) => {
  ...component,
  render: _self => {
    <div className=studentQuestionObjectiveHeader>
      <div className=studentQuestionObjectiveHeaderTextWrapper>
        {ReasonReact.string("Points")}
      </div>
      <div className=studentQuestionObjectiveHeaderTextWrapper>
        {ReasonReact.string("Objective")}
      </div>
      <div className=studentQuestionObjectiveHeaderTextWrapper>{
        ReasonReact.string("Correct")}
      </div>
    </div>
  }
};