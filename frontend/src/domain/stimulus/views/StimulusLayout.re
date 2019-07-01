let component = ReasonReact.statelessComponent("StimulusLayout");

let fullWidth = ReactDOMRe.Style.make(~width="100%", ());

let make =
    (
      ~stimulusId: Stimulus.Model.idType,
      ~normalized,
      ~updateStimulus,
      ~editable,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div style=fullWidth>
      {
        MyNormalizr.Converter.Stimulus.Remote.getRecord(normalized, stimulusId)
        |> Belt.Option.getWithDefault(
             _,
             Stimulus.Model.Record.defaultWithId((), stimulusId),
           )
        |> (
          stimulus =>
            <Stimulus.Mutation.UpdateStimulus>
              ...{
                   (~mutation) =>
                     <Editable
                       editing={editable ? stimulus.local.editingText : false}
                       editingToggle={
                         () => updateStimulus(Stimulus.Action.ToggleEditText)
                       }
                       value={stimulus.data.text}
                       placeholder="Stimulus Goes here."
                       onTextChange={
                         text =>
                           updateStimulus(
                             Stimulus.Action.ApolloUpdateStimulus(
                               () => mutation(~id=stimulus.data.id, ~text),
                             ),
                           )
                       }
                       useTextArea=true
                     />
                 }
            </Stimulus.Mutation.UpdateStimulus>
        )
      }
    </div>,
};