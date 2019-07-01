let component = ReasonReact.statelessComponent("TestBuilderContentSection");
let css = Css.css;
let tw = Css.tw;

let testBuilderContentSection = [%bs.raw
  {| css(tw`
    h-full
    w-full
    p-0
    m-0
    flex
    flex-wrap-reverse
    md:flex-wrap
  `)|}
];

let testBuilderContentSectionContent = [%bs.raw
  {| css(tw`
    w-full
    h-full
    md:w-3/4
  `)|}
  ];

let testBuilderContentSectionSidebar = [%bs.raw
  {| css(tw`
    w-full
    h-full
    md:w-1/4
    overflow-y-scroll
  `)|}
  ];

let testBuilderInternalContentClass = [%bs.raw
  {| css(tw`
    h-full
    w-full
    overflow-y-scroll
  `)|}
];

let testBuilderOuterContentClass = [%bs.raw
{| css(tw`
  mr-4
  h-full
`)|}
];

let make = (~sidebar, children) => {
  ...component,
  render: _self =>
    <div className=testBuilderContentSection>
      <div className=testBuilderContentSectionContent>
        <div className=testBuilderOuterContentClass>
          {
            ReasonReact.createDomElement(
              "div",
              ~props={"className": testBuilderInternalContentClass},
              children,
            )
          }
        </div>
      </div>
      <div className=testBuilderContentSectionSidebar>{sidebar}</div>
    </div>,
};