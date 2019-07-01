module Local = Teacher_Local;
module Model = Teacher_Model;

module ModelSchema = Schema.Teacher;
module Record = ModelSchema.AddModel(Model);

module Action = Teacher_Action;
module Mutation = Teacher_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);