module Model = Classroom_Model;

module ModelSchema = Schema.Classroom;
module Record = ModelSchema.AddModel(Model);

module Action = Classroom_Action;
module Local = Classroom_Local;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);

module Mutation = Classroom_Mutation;