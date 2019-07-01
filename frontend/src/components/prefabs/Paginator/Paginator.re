let component = ReasonReact.statelessComponent("Paginator");

let flexLeft =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="flex-start", ());

let fullWidth = ReactDOMRe.Style.make(~width="100%", ());

let findMin = (numSelections, paginatorSize, selectedIndex) => {
  let width = paginatorSize / 2;

  switch (selectedIndex) {
  | selectedIndex when selectedIndex - width < 0 => 0
  | selectedIndex when selectedIndex + width > numSelections =>
    max(0, numSelections - paginatorSize)
  | selectedIndex => selectedIndex - width
  };
};

let paginatorSize = 5;

let make =
    (
      ~selectedId: 'a,
      ~selections: PaginatorType.selections('a),
      ~onSelect: 'a => 'b,
      _children,
    ) => {
  ...component,
  render: _self => {
    let optionSelection =
      selections
      |> Belt.List.mapWithIndex(_, (idx, selection) => (idx, selection))
      |> Belt.List.getBy(_, ((_, selection)) => selection.id == selectedId);

    let (selectionIdx, selectedSelection) =
      Belt.Option.getWithDefault(
        optionSelection,
        ((-1), {id: selectedId, text: ""}),
      );

    let min =
      findMin(Belt.List.length(selections), paginatorSize, selectionIdx);

    let max = min + paginatorSize - 1;

    let allLeft =
      selections
      |> Belt.List.mapWithIndex(_, (idx, selection) => (idx, selection))
      |> Belt.List.keep(_, ((idx, _)) => idx < selectionIdx)
      |> Belt.List.map(_, ((_, selection)) => selection);

    let prevSelectionMaybe = allLeft |> Belt.List.reverse |> Belt.List.head;

    let leftSelection =
      allLeft
      |> Belt.List.reverse
      |> Belt.List.take(_, selectionIdx - min)
      |> Belt.Option.mapWithDefault(_, allLeft, Belt.List.reverse);

    let allRight =
      selections
      |> Belt.List.mapWithIndex(_, (idx, selection) => (idx, selection))
      |> Belt.List.keep(_, ((idx, _)) => idx > selectionIdx)
      |> Belt.List.map(_, ((_, selection)) => selection);

    let nextSelectionMaybe = Belt.List.head(allRight);

    let rightSelection =
      allRight
      |> Belt.List.take(_, max - selectionIdx)
      |> Belt.Option.getWithDefault(_, allRight);

    let paginatorButton = (selection: PaginatorType.selectionType('a)) =>
      <PaginatorButton
        key={selection.text}
        selection
        isSelected={selectedSelection.id == selection.id}
        onClick={
          _ =>
            (selectedSelection.id != selection.id ? onSelect(selection.id) : ())
            |> ignore
        }
      />;

    <div style=fullWidth>
      {
        Belt.List.length(selections) != 0 ?
          <div style=flexLeft>
            {
              Belt.Option.mapWithDefault(prevSelectionMaybe, <div />, selection =>
                paginatorButton({...selection, text: "prev"})
              )
            }
            {
              leftSelection
              |> Belt.List.map(_, paginatorButton)
              |> Utils.ReasonReact.listToReactArray
            }
            {
              switch (optionSelection) {
              | None => <div />
              | Some(_) => paginatorButton(selectedSelection)
              }
            }
            {
              rightSelection
              |> Belt.List.map(_, paginatorButton)
              |> Utils.ReasonReact.listToReactArray
            }
            {
              Belt.Option.mapWithDefault(nextSelectionMaybe, <div />, selection =>
                paginatorButton({...selection, text: "next"})
              )
            }
          </div> :
          <div />
      }
    </div>;
  },
};