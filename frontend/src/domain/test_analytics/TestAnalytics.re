module Local = TestAnalytics_Local;
module Model = TestAnalytics_Model;

module ModelSchema = Schema.TestAnalytics;
module Record = ModelSchema.AddModel(Model);

module Action = TestAnalytics_Action;
module Mutation = TestAnalytics_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);