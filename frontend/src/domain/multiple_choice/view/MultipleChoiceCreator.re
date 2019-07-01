let component = ReasonReact.statelessComponent("MultipleChoiceCreator");

let fullWidth = ReactDOMRe.Style.make(~width="100%", ());

let make =
    (
      ~updateMultipleChoice,
      ~multipleChoiceId,
      ~updateAddChoice,
      ~normalized,
      ~onSelect,
      ~selectedId,
      _children,
    ) => {
  ...component,
  render: _self => {
    let multipleChoice =
      normalized
      |> MyNormalizr.Converter.MultipleChoice.Remote.getRecord(
           _,
           multipleChoiceId,
         )
      |> Belt.Option.getWithDefault(
           _,
           MultipleChoice.Model.Record.defaultWithId((), multipleChoiceId),
         );

    <div style=fullWidth>
      <MultipleChoiceLayout multipleChoiceId normalized onSelect selectedId />
      <TextInput
        key={multipleChoice.local.newOption.id}
        placeholder="Add option"
        value={multipleChoice.local.newOption.text}
        onTextChange={
          a => updateMultipleChoice(MultipleChoice.Action.EditNewOption(a))
        }
        onKeyDown={
          GenericInput.enterSubmitWithCommand(
            () =>
              updateAddChoice(
                ~id=UUID.generateUUID(),
                ~text=multipleChoice.local.newOption.text,
              ),
            () => updateMultipleChoice(MultipleChoice.Action.NoOp),
          )
        }
      />
    </div>;
  },
};