module Model = StudentAnswerKey_Model;

module ModelSchema = Schema.StudentAnswerKey;
module Record = ModelSchema.AddModel(Model);

module Action = StudentAnswerKey_Action;
module Mutation = StudentAnswerKey_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);