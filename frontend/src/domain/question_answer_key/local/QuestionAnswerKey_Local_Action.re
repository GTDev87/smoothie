type action =
  | ToggleEditScore;

type model = QuestionAnswerKey_Local_Model.Record.t;

let reduce = (action, local: model): model => {
  let {editingScore}: model = local;

  switch (action) {
  | ToggleEditScore => {editingScore: !editingScore}
  };
};