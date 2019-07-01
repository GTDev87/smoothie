module Model = StudentTest_Model;

module ModelSchema = Schema.StudentTest;
module Record = ModelSchema.AddModel(Model);

module Action = StudentTest_Action;
module Mutation = StudentTest_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);