type action =
  | NoOp;

type model = Profile_Local_Model.Record.t;

let reduce = (action, local: model): model =>
  switch (action) {
  | NoOp => local
  };