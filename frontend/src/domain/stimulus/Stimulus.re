module Model = Stimulus_Model;

module ModelSchema = Schema.Stimulus;
module Record = ModelSchema.AddModel(Model);

module Action = Stimulus_Action;
module Mutation = Stimulus_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);