let component = ReasonReact.statelessComponent("PaginatorButton");

let paginatorButtonStyle = selected =>
  ReactDOMRe.Style.make(
    ~backgroundColor=selected ? Colors.primary : "white",
    ~border="solid 1px grey",
    ~padding="0 .3em",
    ~margin="0 .1em",
    ~cursor="pointer",
    (),
  );

let make =
    (
      ~selection: PaginatorType.selectionType('a),
      ~isSelected: bool,
      ~onClick,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div
      key={selection.text} style={paginatorButtonStyle(isSelected)} onClick>
      {ReasonReact.string(selection.text)}
    </div>,
};