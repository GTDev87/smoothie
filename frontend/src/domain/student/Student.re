module Model = Student_Model;

module ModelSchema = Schema.Student;
module Record = ModelSchema.AddModel(Model);

module Action = Student_Action;
module Mutation = Student_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);