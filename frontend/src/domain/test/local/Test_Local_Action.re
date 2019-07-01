type action =
  | ResetNewQuestion
  | ResetNewObjective
  | ToggleEditName
  | ToggleEditDescription
  | ToggleEditNotes
  | SelectResultTab(ResultTab.t)
  | ChangeSelectedQuestion(option(UUID.t))
  | ChangeGraphDisplay(OverviewGraphDisplay.t)
  | ChangeQuestionTabType(QuestionTab.t);

type model = Test_Local_Model.Record.t;

let reduce = (action, local: model): model => {
  let {editingName, editingNotes, editingDescription}: model = local;
  switch (action) {
  | ResetNewQuestion => {
      ...local,
      newQuestionId: LongAnswer(UUID.generateUUID()),
    }
  | ResetNewObjective => {
      ...local,
      newObjectiveId: Schema.Objective.stringToId(UUID.generateUUID()),
    }
  | ToggleEditName => {...local, editingName: !editingName}
  | ToggleEditNotes => {...local, editingNotes: !editingNotes}
  | ToggleEditDescription => {
      ...local,
      editingDescription: !editingDescription,
    }
  | SelectResultTab(resultTab) => {...local, resultTab}
  | ChangeSelectedQuestion(selectedQuestion) => {...local, selectedQuestion}
  | ChangeGraphDisplay(overviewGraphDisplay) => {
      ...local,
      overviewGraphDisplay,
    }
  | ChangeQuestionTabType(questionTab) => {...local, questionTab}
  };
};