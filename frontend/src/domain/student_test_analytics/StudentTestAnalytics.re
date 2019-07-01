module Local = StudentTestAnalytics_Local;
module Model = StudentTestAnalytics_Model;

module ModelSchema = Schema.StudentTestAnalytics;
module Record = ModelSchema.AddModel(Model);

module Action = StudentTestAnalytics_Action;
module Mutation = StudentTestAnalytics_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);