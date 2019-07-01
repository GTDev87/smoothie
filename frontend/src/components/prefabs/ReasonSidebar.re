let fullWidth = ReactDOMRe.Style.make(~width="100%", ());

let css = Css.css;
let tw = Css.tw;

let reasonSidebarMainClass = [%bs.raw {| css(tw` w-full h-full`)|}];
let contentHeaderMenuLinkStyles =
  ReactDOMRe.Style.make(
    ~textDecoration="none",
    ~color="red",
    ~padding="8px",
    ~fontSize="20px",
    (),
  );

type state = {
  mql: option(Document.mediaQuery),
  docked: bool,
  isOpen: bool,
};

type action =
  | SetOpen(bool)
  | ToggleOpen
  | MediaChange(Document.mediaQuery);

let component = ReasonReact.reducerComponent("ReasonSidebar");

let make = (~sidebar, ~style=?, ~header, ~contentStyle=?, children) => {
  ...component,
  initialState: () => {mql: None, docked: false, isOpen: false},
  reducer: (action, state: state) =>
    switch (action) {
    | SetOpen(isOpen) => ReasonReact.Update({...state, isOpen})
    | ToggleOpen => ReasonReact.Update({...state, isOpen: !state.isOpen})
    | MediaChange(mqlVal) =>
      ReasonReact.Update({
        ...state,
        mql: Some(mqlVal),
        docked: Document.matches(mqlVal),
      })
    },
  didMount: ({send}) =>
    Document.isBrowser() ?
      {
        let mql = Document.matchMedia("(min-width: 800px)");
        Document.addListener(mql, () => send(MediaChange(mql)));
        send(MediaChange(mql));
        ();
      } :
      (),
  willUnmount: ({state, send}) => {
    let {mql} = state;
    Belt.Option.mapWithDefault(mql, (), mql =>
      Document.removeListener(mql, () => send(MediaChange(mql)))
    );
  },
  render: self =>
    <ReactSidebar
      onSetOpen={isOpen => self.send(SetOpen(isOpen))}
      sidebar
      isOpen={self.state.isOpen}
      isDocked={self.state.docked}
      ?style
      ?contentStyle>
      <div style=fullWidth>
        {
          !self.state.docked ?
            <a
              onClick={_ => self.send(ToggleOpen)}
              href="#"
              style=contentHeaderMenuLinkStyles>
              {ReasonReact.string("=")}
            </a> :
            <div />
        }
        header
      </div>
      {
        ReasonReact.createDomElement(
          "div",
          ~props={"className": reasonSidebarMainClass},
          children,
        )
      }
    </ReactSidebar>,
};