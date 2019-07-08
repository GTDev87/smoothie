let component = ReasonReact.statelessComponent("ProfileLayout");

let fullWidth =
  ReactDOMRe.Style.make(~width="100%", ~padding="0", ~margin="0", ());

let css = Css.css;
let tw = Css.tw;

let teacherMainContentClass = [%bs.raw
  {| css(tw` w-full h-full pt-4 pl-8 `)|}
];

let flexCenter =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~alignItems="center",
    (),
  );

let textStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="space-between",
    ~paddingLeft=".5em",
    (),
  );

let widthStyle = "15%";

let sidebarStyle =
  ReactDOMRe.Style.make(
    ~width=widthStyle,
    ~backgroundColor=Colors.primary,
    ~padding="0",
    ~color="white",
    (),
  );

let realAndOpenStyle = ReactDOMRe.Style.make(~padding="1em", ());

let fullSizeStyle = ReactDOMRe.Style.make(~height="100%", ~width="100%", ());

let make = (_children) => {
  ...component,
  render: _self =>
    <div>
    </div>,
};