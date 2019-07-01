module Model = QuestionBase_Model;

module ModelSchema = Schema.QuestionBase;
module Record = ModelSchema.AddModel(Model);

module Action = QuestionBase_Action;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);