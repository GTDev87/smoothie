module Model = Grade_Model;

module ModelSchema = Schema.Grade;
module Record = ModelSchema.AddModel(Model);

module Container = ApolloFragment.Container(ApolloClient.ReadFragment, Model);