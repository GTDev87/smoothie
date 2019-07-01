module Model = MultipleChoiceQuestion_Model;

module ModelSchema = Schema.MultipleChoiceQuestion;
module Record = ModelSchema.AddModel(Model);

module Action = MultipleChoiceQuestion_Action;
module Mutation = MultipleChoiceQuestion_Mutation;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);