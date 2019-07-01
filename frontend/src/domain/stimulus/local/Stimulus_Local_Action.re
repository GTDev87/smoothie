type action =
  | ToggleEditText;

type model = Stimulus_Local_Model.Record.t;

let reduce = (action, local: model): model => {
  let {editingText}: model = local;

  switch (action) {
  | ToggleEditText => {editingText: !editingText}
  };
};