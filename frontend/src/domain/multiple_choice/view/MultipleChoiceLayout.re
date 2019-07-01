let component = ReasonReact.statelessComponent("MultipleChoiceLayout");

let css = Css.css;
let tw = Css.tw;

let multipleChoiceAnswerClass = [%bs.raw {| css(tw`
  bg-green-lightest
  border-blue-lighter
  border
  border-solid
`)|}];

let make = (
  ~multipleChoiceId,
  ~normalized,
  ~onSelect,
  ~selectedId,
  ~answer:option(string)=None,
  _children
) => {
  ...component,
  render: _self => {
    let multipleChoice =
      normalized
      |> MyNormalizr.Converter.MultipleChoice.Remote.getRecord(_, multipleChoiceId)
      |> Belt.Option.getWithDefault(_, MultipleChoice.Model.Record.defaultWithId((), multipleChoiceId));
         
    let answerFunction = Belt.Option.mapWithDefault(
      answer,
      ((_) => ""),
      (answer) => (
        (sel) => (sel == answer ? multipleChoiceAnswerClass : "")));

    <MultipleChoiceSelector
      key={multipleChoice.data.id}
      selectionClass=(answerFunction)
      selectedId
      selections={
        Belt.List.map(multipleChoice.data.choices, choice =>
          (
            {id: choice.text, text: choice.text}:
              MultipleChoiceSelector.selectionType(string)
          )
        )
      }
      onSelect
    />;
  },
};