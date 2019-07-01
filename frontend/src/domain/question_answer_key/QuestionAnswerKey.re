module Model = QuestionAnswerKey_Model;

module ModelSchema = Schema.QuestionAnswerKey;
module Record = ModelSchema.AddModel(Model);

module Action = QuestionAnswerKey_Action;
module Mutation = QuestionAnswerKey_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);