let component = ReasonReact.statelessComponent("IconText");

let css = Css.css;
let tw = Css.tw;
let fileIconClass = [%bs.raw {| css(tw` mx-24 my-12`)|}];
let iconClass = [%bs.raw
  {| css(tw` text-5xl flex my-2 justify-center text-blue-light `)|}
];
let textClass = [%bs.raw {| css(tw` text-blue-light text-center `)|}];

let make = (~icon, ~text, _children) => {
  ...component,
  render: _self =>
    <div className=fileIconClass>
      <div className=iconClass> icon </div>
      <div className=textClass> {ReasonReact.string(text)} </div>
    </div>,
};