let component = ReasonReact.statelessComponent("TestTooltip");

let question_truncate = 30;

let make = (~testNumber=?, ~testText, ~maxTextLength=?, _children) => {
  ...component,
  render: _self => {
    let length = Belt.Option.getWithDefault(maxTextLength, question_truncate);

    let textComponent =
      ReasonReact.string(
        (
          switch (testNumber) {
          | None => ""
          | Some(testNumberVal) =>
            "Q." ++ (testNumberVal |> string_of_int) ++ " "
          }
        )
        ++ (testText |> Js.String.slice(~from=0, ~to_=length)),
      );

    String.length(testText) != 0 ?
      <Tooltip tooltipText=testText> textComponent </Tooltip> : textComponent;
  },
};