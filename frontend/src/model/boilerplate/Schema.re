module ModelGenerator = ModelUtils.GenerateModel(ModelUtils.RootModel);

module Teacher = ModelGenerator();
module Classroom = ModelGenerator();
module Student = ModelGenerator();
module StudentTest = ModelGenerator();
module Test = ModelGenerator();
module QuestionBase = ModelGenerator();
module LongAnswerQuestion = ModelGenerator();
module MultipleChoiceQuestion = ModelGenerator();
module Stimulus = ModelGenerator();
module QuestionAnswerKey = ModelGenerator();
module StudentQuestion = ModelGenerator();
module StudentAnswerKey = ModelGenerator();
module Objective = ModelGenerator();
module MultipleChoice = ModelGenerator();
module User = ModelGenerator();
module Grade = ModelGenerator();
module TrueFalseQuestion = ModelGenerator();
module FillInTheBlankQuestion = ModelGenerator();
module TestAnalytics = ModelGenerator();
module StudentTestAnalytics = ModelGenerator();
module ObjectiveAnalytics = ModelGenerator();
module AnswerKeyAnalytics = ModelGenerator();

/* Generate by Normalizr */

/* can come from normalizr module */
module Question = {
  type id =
    | LongAnswer(UUID.t)
    | MultipleChoice(UUID.t)
    | TrueFalse(UUID.t)
    | FillInTheBlank(UUID.t);

  let idToString = (questionId: id) => {
    switch(questionId){
    | LongAnswer(uuid) => uuid
    | MultipleChoice(uuid) => uuid
    | TrueFalse(uuid) => uuid
    | FillInTheBlank(uuid) => uuid
    }
  }
};

let getQuestionUUIDFromId = (qid: Question.id): UUID.t =>
  switch (qid) {
  | LongAnswer(id) => id
  | MultipleChoice(id) => id
  | TrueFalse(id) => id
  | FillInTheBlank(id) => id
  };
