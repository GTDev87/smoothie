module Model = LongAnswerQuestion_Model;

module ModelSchema = Schema.LongAnswerQuestion;
module Record = ModelSchema.AddModel(Model);

module Action = LongAnswerQuestion_Action;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);