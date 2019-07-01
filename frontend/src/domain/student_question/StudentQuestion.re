module Model = StudentQuestion_Model;

module ModelSchema = Schema.StudentQuestion;
module Record = ModelSchema.AddModel(Model);

module Action = StudentQuestion_Action;
module Mutation = StudentQuestion_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);