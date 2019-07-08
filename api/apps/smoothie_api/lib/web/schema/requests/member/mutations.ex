defmodule SmoothieApi.Web.Schema.Member.Mutations do
  use Absinthe.Schema.Notation
  import_types(SmoothieApi.Web.Schema.Member.Mutations.User)

  object :member_mutation do
    import_fields(:user_member_mutations)
  end
end
