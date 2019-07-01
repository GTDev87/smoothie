module Local = ObjectiveAnalytics_Local;
module Model = ObjectiveAnalytics_Model;

module ModelSchema = Schema.ObjectiveAnalytics;
module Record = ModelSchema.AddModel(Model);

module Action = ObjectiveAnalytics_Action;
module Mutation = ObjectiveAnalytics_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);