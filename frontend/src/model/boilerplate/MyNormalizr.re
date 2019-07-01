
module FullReduced =
  ModelUtils.AddRecord(
    ModelUtils.AddRecord(
      ModelUtils.AddRecord(
        ModelUtils.AddRecord(
          ModelUtils.AddRecord(
            ModelUtils.AddRecord(
              ModelUtils.AddRecord(
                ModelUtils.AddRecord(
                  ModelUtils.AddRecord(
                    ModelUtils.AddRecord(
                      ModelUtils.AddRecord(
                        ModelUtils.AddRecord(
                          ModelUtils.AddRecord(
                            ModelUtils.AddRecord(
                              ModelUtils.AddRecord(
                                ModelUtils.AddRecord(
                                  ModelUtils.AddRecord(
                                    ModelUtils.AddRecord(
                                      ModelUtils.AddRecord(
                                        ModelUtils.AddRecord(
                                          ModelUtils.AddRecord(
                                            ModelUtils.AddRecord(
                                              ModelUtils.EmptyNormalizr(ModelUtils.RootModel),
                                            AnswerKeyAnalytics.Record),
                                          ObjectiveAnalytics.Record),
                                        StudentTestAnalytics.Record),
                                      Teacher.Record),
                                    TestAnalytics.Record),
                                  Classroom.Record), 
                                Student.Record), 
                              StudentTest.Record),
                            Test.Record),
                          LongAnswerQuestion.Record),
                        FillInTheBlankQuestion.Record),
                      QuestionBase.Record),
                    MultipleChoiceQuestion.Record),
                  TrueFalseQuestion.Record),
                Stimulus.Record),
              QuestionAnswerKey.Record),
            StudentQuestion.Record),
          StudentAnswerKey.Record),
        Objective.Record),
      MultipleChoice.Record),
    User.Record),
  Grade.Record
);

module Converter = {
  module Teacher = NormalizrSetup.DomainTypeConverter(Teacher, FullReduced, Teacher.Container, Teacher.Record.Wrapper);
  module Classroom = NormalizrSetup.DomainTypeConverter(Classroom, FullReduced, Classroom.Container, Classroom.Record.Wrapper);
  module Student = NormalizrSetup.DomainTypeConverter(Student, FullReduced, Student.Container, Student.Record.Wrapper);
  module StudentTest = NormalizrSetup.DomainTypeConverter(StudentTest, FullReduced, StudentTest.Container, StudentTest.Record.Wrapper);
  module Test = NormalizrSetup.DomainTypeConverter(Test, FullReduced, Test.Container, Test.Record.Wrapper);
  module QuestionBase =
    NormalizrSetup.DomainTypeConverter(QuestionBase, FullReduced, QuestionBase.Container, QuestionBase.Record.Wrapper);
  module MultipleChoiceQuestion =
    NormalizrSetup.DomainTypeConverter(
      MultipleChoiceQuestion,
      FullReduced, 
      MultipleChoiceQuestion.Container,
      MultipleChoiceQuestion.Record.Wrapper,
    );
  module LongAnswerQuestion =
    NormalizrSetup.DomainTypeConverter(LongAnswerQuestion, FullReduced, LongAnswerQuestion.Container, LongAnswerQuestion.Record.Wrapper);
  module Stimulus = NormalizrSetup.DomainTypeConverter(Stimulus, FullReduced, Stimulus.Container, Stimulus.Record.Wrapper);
  module QuestionAnswerKey =
    NormalizrSetup.DomainTypeConverter(QuestionAnswerKey, FullReduced, QuestionAnswerKey.Container, QuestionAnswerKey.Record.Wrapper);
  module StudentQuestion =
    NormalizrSetup.DomainTypeConverter(StudentQuestion, FullReduced, StudentQuestion.Container, StudentQuestion.Record.Wrapper);
  module StudentAnswerKey =
    NormalizrSetup.DomainTypeConverter(StudentAnswerKey, FullReduced, StudentAnswerKey.Container, StudentAnswerKey.Record.Wrapper);
  module Objective = NormalizrSetup.DomainTypeConverter(Objective, FullReduced, Objective.Container, Objective.Record.Wrapper);
  module MultipleChoice =
    NormalizrSetup.DomainTypeConverter(MultipleChoice, FullReduced, MultipleChoice.Container, MultipleChoice.Record.Wrapper);
  module TrueFalseQuestion =
    NormalizrSetup.DomainTypeConverter(TrueFalseQuestion, FullReduced, TrueFalseQuestion.Container, TrueFalseQuestion.Record.Wrapper);
  module FillInTheBlankQuestion =
    NormalizrSetup.DomainTypeConverter(FillInTheBlankQuestion, FullReduced, FillInTheBlankQuestion.Container, FillInTheBlankQuestion.Record.Wrapper);
  module TestAnalytics =
    NormalizrSetup.DomainTypeConverter(TestAnalytics, FullReduced, TestAnalytics.Container, TestAnalytics.Record.Wrapper);
  module StudentTestAnalytics =
    NormalizrSetup.DomainTypeConverter(StudentTestAnalytics, FullReduced, StudentTestAnalytics.Container, StudentTestAnalytics.Record.Wrapper);
  module ObjectiveAnalytics =
    NormalizrSetup.DomainTypeConverter(ObjectiveAnalytics, FullReduced, ObjectiveAnalytics.Container, ObjectiveAnalytics.Record.Wrapper);
  module AnswerKeyAnalytics =
    NormalizrSetup.DomainTypeConverter(AnswerKeyAnalytics, FullReduced, AnswerKeyAnalytics.Container, AnswerKeyAnalytics.Record.Wrapper);
};

let localGetQuestionFromSchema =
    (normalized, id): option(Question.Model.Record.t) =>
  switch (id) {
  | Schema.Question.LongAnswer(id) =>
    Converter.LongAnswerQuestion.Local.getRecord(normalized, Schema.LongAnswerQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.LongAnswerQuestion(q))
  | Schema.Question.MultipleChoice(id) =>
    Converter.MultipleChoiceQuestion.Local.getRecord(normalized, Schema.MultipleChoiceQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.MultipleChoiceQuestion(q))
  | Schema.Question.TrueFalse(id) =>
    Converter.TrueFalseQuestion.Local.getRecord(normalized, Schema.TrueFalseQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.TrueFalseQuestion(q))
  | Schema.Question.FillInTheBlank(id) =>
    Converter.FillInTheBlankQuestion.Local.getRecord(normalized, Schema.FillInTheBlankQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.FillInTheBlankQuestion(q))
  };

let getQuestionFromSchema = (normalized, id): option(Question.Model.Record.t) =>
  switch (id) {
  | Schema.Question.LongAnswer(id) =>
    Converter.LongAnswerQuestion.Remote.getRecord(normalized, Schema.LongAnswerQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.LongAnswerQuestion(q))
  | Schema.Question.MultipleChoice(id) =>
    Converter.MultipleChoiceQuestion.Remote.getRecord(normalized, Schema.MultipleChoiceQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.MultipleChoiceQuestion(q))
  | Schema.Question.TrueFalse(id) =>
    Converter.TrueFalseQuestion.Remote.getRecord(normalized, Schema.TrueFalseQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.TrueFalseQuestion(q))
  | Schema.Question.FillInTheBlank(id) =>
    Converter.FillInTheBlankQuestion.Remote.getRecord(normalized, Schema.FillInTheBlankQuestion.stringToId(id))
    |> Belt.Option.map(_, q => Question.Model.FillInTheBlankQuestion(q))
  };
