module Local = Profile_Local;
module Model = Profile_Model;

module ModelSchema = Schema.Profile;
module Record = ModelSchema.AddModel(Model);

module Action = Profile_Action;
module Mutation = Profile_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);