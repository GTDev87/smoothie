let component = ReasonReact.statelessComponent("TestLayout");

let css = Css.css;
let tw = Css.tw;

let flexFullSizeClass = [%bs.raw
  {| css(tw` flex flex-col w-full h-full p-0 m-0 content-start items-stretch  `)|}
];

let remainingSizeClass = [%bs.raw {| css(tw`
  flex
  flex-col
  justify-between
  w-full
  h-full
  p-0
  m-0
`)|}];

let testLayoutHeaderClass = [%bs.raw
  {| css(tw` w-full p-0 m-0 flex-no-grow`)|}
];

let testLayoutFooterClass = [%bs.raw {| css(tw`
  bg-white
  w-full
  flex-no-grow
`)|}
];

let testLayoutFooterInnerClass = [%bs.raw
{| css(tw`
  pt-2
`)|}
];

let testLayoutContentClass = [%bs.raw
  {| css(tw`
    w-full
    overflow-y-scroll
    flex-1
    relative
    pb-4
  `)|}
];



let mainContentClass = [%bs.raw {| css(tw` absolute pin-t pin-l w-full h-full`)|}];

let make = (~header=?, ~footer, children) => {
  ...component,
  render: _self =>
    <div className=remainingSizeClass>
      <div className=flexFullSizeClass>
        {
          switch (header) {
          | None => <div />
          | Some(header) =>
            <div className=testLayoutHeaderClass>
              <ContentHeader> header </ContentHeader>
            </div>
          }
        }
        <div className=testLayoutContentClass>
          {
            ReasonReact.createDomElement(
              "div",
              ~props={"className": mainContentClass},
              children,
            )
          }
        </div>
        <div className=testLayoutFooterClass>
          <div className=testLayoutFooterInnerClass>footer</div>
        </div>
      </div>
    </div>,
};