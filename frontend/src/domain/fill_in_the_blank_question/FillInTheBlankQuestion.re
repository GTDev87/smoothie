module Model = FillInTheBlankQuestion_Model;

module ModelSchema = Schema.FillInTheBlankQuestion;
module Record = ModelSchema.AddModel(Model);

module Action = FillInTheBlankQuestion_Action;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);