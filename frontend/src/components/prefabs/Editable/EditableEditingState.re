type state = {contentRef: ref(option(Dom.element))};

type action =
  | Nothing;

let component = ReasonReact.reducerComponent("EditableEditingState");

let setContentRef = (r, {ReasonReact.state}) =>
  state.contentRef := Js.Nullable.toOption(r);

let setElementContents = (notFocused, element) => {
  let node = ReactDOMRe.domElementToObj(element);

  ignore(
    Belt.Option.getWithDefault(notFocused, false) == false ?
      node##focus() : (),
  ); /* uncertain about this */
  ignore(node##setSelectionRange(node##value##length, node##value##length));

  ();
};

let editableTextAreaStyle = ReactDOMRe.Style.make(~width="100%", ());

let usingTextArea = useTextArea =>
  switch (useTextArea) {
  | Some(true) => true
  | Some(false) => false
  | None => false
  };

let make =
    (
      ~style=?,
      ~value,
      ~onTextChange,
      ~onKeyDown=?,
      ~className=?,
      ~placeholder,
      ~editingToggle,
      ~useTextArea=?,
      ~notFocused=?,
      _children,
    ) => {
  ...component,
  initialState: () => {contentRef: ref(None)},
  reducer: (_action, _state) =>
    switch (_action) {
    | Nothing => ReasonReact.NoUpdate
    },
  didMount: self =>
    switch (self.state.contentRef^) {
    | Some(field) => setElementContents(notFocused, field)
    | _ => ()
    },
  render: self => {
    let computedStyle = Utils.Dom.combineStyles(style, editableTextAreaStyle);
    let onBlur = _event => editingToggle() |> ignore;

    /* until if figure out how to programatically choose a component*/
    usingTextArea(useTextArea) ?
      <TextArea
        style=computedStyle
        ?onKeyDown
        ?className
        placeholder
        onTextChange
        onBlur
        value
      /> :
      <TextInput
        style=computedStyle
        ?className
        ?onKeyDown
        placeholder
        onTextChange
        onBlur
        value
      />;
  },
};