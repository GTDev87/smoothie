/* done */
type action =
  | NoOp;

type model = User_Model.Record.t;

let reduce = (action, user: model): model =>
  switch (action) {
  /* local below */
  | NoOp => user
  };