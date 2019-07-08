
module FullReduced = ModelUtils.AddRecord(
  ModelUtils.AddRecord(
    ModelUtils.EmptyNormalizr(ModelUtils.RootModel), Profile.Record),
  User.Record
);

module Converter = {
  module Profile = NormalizrSetup.DomainTypeConverter(Profile, FullReduced, Profile.Container, Profile.Record.Wrapper);
  module User = NormalizrSetup.DomainTypeConverter(User, FullReduced, User.Container, User.Record.Wrapper);
};

