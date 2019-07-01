module Model = TrueFalseQuestion_Model;

module ModelSchema = Schema.TrueFalseQuestion;
module Record = ModelSchema.AddModel(Model);

module Action = TrueFalseQuestion_Action;
module Mutation = TrueFalseQuestion_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);