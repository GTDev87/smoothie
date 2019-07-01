let component = ReasonReact.statelessComponent("MultipleChoiceSelector");
let css = Css.css;
let cx = Css.cx;
let tw = Css.tw;

type selectionType('a) = {
  id: 'a,
  text: string,
};
type selections('a) = list(selectionType('a));

let multipleChoiceSelectorClass = [%bs.raw {| css(tw`
  w-full
  py-2
  `)|}];
let rowClass = [%bs.raw {| css(tw`
  w-full
  m-0
  pt-1
  pl-2
  flex
  flex-col
  justify-center
  border-white
  border-solid
  `)|}];
let radioInputClass = [%bs.raw {| css(tw` mr-4`)|}];

let make =
    (
      ~selectedId: 'a,
      ~selections: selections('a),
      ~onSelect: 'a => 'b,
      ~selectionClass:(('a) => string) = ((a) => ""),
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className=multipleChoiceSelectorClass>
      {
        selections
        |> Belt.List.map(_, selection =>
             <div key={selection.id} className=cx(rowClass, selectionClass(selection.id))>
              <div>
               <input
                 _type="radio"
                 className=radioInputClass
                 value="option1"
                 id={selection.text}
                 checked={selection.id == selectedId}
                 onChange={_ => onSelect(selection.id) |> ignore}
               />
               <label htmlFor={selection.text}>
                 {ReasonReact.string(selection.text)}
               </label>
              </div>
             </div>
           )
        |> Utils.ReasonReact.listToReactArray
      }
    </div>,
};