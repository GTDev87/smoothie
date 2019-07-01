let component = ReasonReact.statelessComponent("StudentMasteryList");

let correctAnswerStyle = correct =>
  ReactDOMRe.Style.make(
    ~fontSize="1em",
    ~color=correct ? Colors.correct : Colors.incorrect,
    (),
  );

let make = (~listName, ~correct, ~studentList: list(string), _children) => {
  ...component,
  render: _self =>
    <div>
      <h3 style={correctAnswerStyle(correct)}>
        {ReasonReact.string(listName)}
      </h3>
      <div>
        {
          Belt.List.length(studentList) !== 0 ?
            studentList
            |> Utils.List.joinStringList(_, ", ")
            |> ReasonReact.string :
            ReasonReact.string("None")
        }
      </div>
      <br />
    </div>,
};