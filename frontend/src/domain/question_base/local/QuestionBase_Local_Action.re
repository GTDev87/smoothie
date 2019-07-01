type action =
  | ResetNewAnswerKey
  | ToggleEditText
  | ToggleEditSolution;

type model = QuestionBase_Local_Model.Record.t;

let reduce = (action, local: model): model => {
  let {editingText, editingSolution}: model = local;

  switch (action) {
  | ResetNewAnswerKey => {
      ...local,
      newAnswerKeyId: Schema.QuestionAnswerKey.stringToId(UUID.generateUUID()),
    }
  | ToggleEditText => {...local, editingText: !editingText}
  | ToggleEditSolution => {...local, editingSolution: !editingSolution}
  };
};