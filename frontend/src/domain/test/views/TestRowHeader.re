let component = ReasonReact.statelessComponent("TestRowHeader");
let css = Css.css;
let tw = Css.tw;
let testRowHeaderClass = [%bs.raw
  {| css(tw`
    flex
  `)|}
];

let testRowHeaderSmallTextClass = [%bs.raw
  {| css(tw`
    w-1/6
  `)|}
];

let testRowHeaderLongTextClass = [%bs.raw
  {| css(tw`
    w-2/3
  `)|}
];

let testRowHeaderTextPaddingClass = [%bs.raw
  {| css(tw`
    px-4
  `)|}
];

let make = (_children) => {
  ...component,
  render: _self =>
    <div className=testRowHeaderClass>
      <div className=testRowHeaderSmallTextClass>
        <div className=testRowHeaderTextPaddingClass>
          {ReasonReact.string("Test Name")}
        </div>
      </div>
      <div className=testRowHeaderSmallTextClass>
        <div className=testRowHeaderTextPaddingClass>
          {ReasonReact.string("# Questions")}
        </div>
      </div>
      <div className=testRowHeaderLongTextClass>
        <div className=testRowHeaderTextPaddingClass>
          {ReasonReact.string("Objectives")}
        </div>
      </div>
    </div>,
};