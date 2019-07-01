module Model = Test_Model;

module ModelSchema = Schema.Test;
module Record = ModelSchema.AddModel(Model);

module Action = Test_Action;
module Mutation = Test_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);