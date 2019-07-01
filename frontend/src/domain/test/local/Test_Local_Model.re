type _record = {
  newQuestionId: Schema.Question.id,
  newObjectiveId: Schema.Objective.id,
  editingName: bool,
  editingDescription: bool,
  editingNotes: bool,
  resultTab: ResultTab.t,
  questionTab: QuestionTab.t,
  overviewGraphDisplay: OverviewGraphDisplay.t,
  selectedQuestion: option(UUID.t),
};

let _defaultRecord = id => {
  module UUIDSeedType = {
    let namespace = id;
  };

  module Rand = UUID.V5Random(UUIDSeedType);
  {
    newQuestionId: LongAnswer(Rand.generateSeqUUID()),
    newObjectiveId: Schema.Objective.stringToId(Rand.generateSeqUUID()),
    editingName: false,
    editingDescription: false,
    editingNotes: false,
    resultTab: ResultTab.Overview,
    questionTab: QuestionTab.Overview,
    overviewGraphDisplay: OverviewGraphDisplay.Mastery,
    selectedQuestion: None,
  };
};

module Record = {
  type t = _record;
  let default = _defaultRecord;
};