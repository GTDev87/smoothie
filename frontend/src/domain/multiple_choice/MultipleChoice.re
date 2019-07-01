module Model = MultipleChoice_Model;

module ModelSchema = Schema.MultipleChoice;
module Record = ModelSchema.AddModel(Model);

module Action = MultipleChoice_Action;
module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);