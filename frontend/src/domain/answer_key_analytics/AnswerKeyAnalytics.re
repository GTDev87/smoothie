module Local = AnswerKeyAnalytics_Local;
module Model = AnswerKeyAnalytics_Model;

module ModelSchema = Schema.AnswerKeyAnalytics;
module Record = ModelSchema.AddModel(Model);

module Action = AnswerKeyAnalytics_Action;
module Mutation = AnswerKeyAnalytics_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);