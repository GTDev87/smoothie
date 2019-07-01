type action =
  | ToggleEditFirstName
  | ToggleEditLastName;

type model = Student_Local_Model.Record.t;

let reduce = (action, local: model): model => {
  let {edittingFirstName, edittingLastName}: model = local;

  switch (action) {
  | ToggleEditFirstName => {...local, edittingFirstName: !edittingFirstName}
  | ToggleEditLastName => {...local, edittingLastName: !edittingLastName}
  };
};