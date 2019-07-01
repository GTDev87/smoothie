let component = ReasonReact.statelessComponent("TestRow");

let css = Css.css;
let tw = Css.tw;
let testRowClass = [%bs.raw
  {| css(tw`
    py-2
    w-full
    flex
  `)|}
];

let testRowSmallTextClass = [%bs.raw
  {| css(tw`
    w-1/6
  `)|}
];

let testRowLongTextClass = [%bs.raw
  {| css(tw`
    w-1/2
  `)|}
];

let testRowTextPaddingClass = [%bs.raw
  {| css(tw`
    px-4
  `)|}
];

let testRowButtonClass = [%bs.raw
{| css(tw`
  w-full
  h-full
  flex
  justify-center
  items-center
`)|}
];

let testRowContent = ReactDOMRe.Style.make(~margin="1em", ~color="grey", ());

let make = (~data as test: Test.Model.Record.t, ~onClick, ~normalized, _children) => {
  ...component,
  render: _self =>
    <div className=testRowClass>
      <div className=testRowSmallTextClass>
        <div className=testRowTextPaddingClass>
          {ReasonReact.string(test.data.name)}
        </div>
      </div>
      <div className=testRowSmallTextClass>
        <div className=testRowTextPaddingClass>
          {
            test.data.questionIds
            |> Belt.List.length
            |> string_of_int
            |> ReasonReact.string
          }
        </div>
      </div>
      <div className=testRowLongTextClass>
        <div className=testRowTextPaddingClass>
          {
            (
              Belt.List.length(test.data.objectiveIds) !== 0 ?
                (
                  test.data.objectiveIds
                  |> Utils.List.removeOptionsFromList
                  |> MyNormalizr.Converter.Objective.idListToFilteredItems(
                      _,
                      MyNormalizr.Converter.Objective.Remote.getRecord(normalized),
                    )
                  |> Belt.List.map(_, obj => obj.data.text)
                  |> Utils.List.joinStringList(_, ", ")
                ) :
                ""
          )
          |> ReasonReact.string
        }
      </div>
    </div>
    <div className=testRowSmallTextClass>
      <div className=testRowButtonClass> 
        <Button onClick={_ => onClick() |> ignore}>
          { ReasonReact.string("Edit") }
        </Button>
      </div>
    </div>
  </div>
};
