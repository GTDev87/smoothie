defmodule AssessmentApi.Web.Schema.Provider.Mutations do
  use Absinthe.Schema.Notation
  import_types(AssessmentApi.Web.Schema.Provider.Mutations.User)

  object :provider_mutation do
    import_fields(:user_provider_mutations)
  end
end
