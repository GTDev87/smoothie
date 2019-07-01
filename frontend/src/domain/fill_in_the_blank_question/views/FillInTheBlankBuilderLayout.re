let component = ReasonReact.statelessComponent("FillInTheBlankBuilderLayout");

let make =
    (
      ~data as fillInTheBlankQuestion: FillInTheBlankQuestion.Model.Record.t,
      _children,
    ) => {
  ...component,
  render: _self => {
    <div />
  }
};