module Model = Objective_Model;

module ModelSchema = Schema.Objective;
module Record = ModelSchema.AddModel(Model);

module Action = Objective_Action;
module Mutation = Objective_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);