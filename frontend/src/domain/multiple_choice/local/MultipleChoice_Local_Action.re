type action =
  | EditNewOption(string)
  | ResetOption;

type model = MultipleChoice_Local_Model.Record.t;

let reduce = (action, local: model): model =>
  switch (action) {
  | EditNewOption(optionText) => {
      newOption: {
        text: optionText,
        id: local.newOption.id,
      },
    }
  | ResetOption => {
      newOption: {
        id: UUID.generateUUID(),
        text: "",
      },
    }
  };