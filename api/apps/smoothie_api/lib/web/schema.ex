defmodule SmoothieApi.Web.Schema do
  use Absinthe.Schema

  import_types(SmoothieApi.Web.Types)

  import_types(SmoothieApi.Web.Schema.Provider.Mutations)
  import_types(SmoothieApi.Web.Schema.Provider.Queries)

  import_types(SmoothieApi.Web.Schema.Member.Mutations)
  import_types(SmoothieApi.Web.Schema.Member.Queries)

  import_types(SmoothieApi.Web.Schema.General.Queries)

  query do
    import_fields(:member_query)
    import_fields(:general_query)
  end

  mutation do
    import_fields(:member_mutation)
    import_fields(:provider_mutation)
  end

  subscription do
    
  end

  def plugins do
    [Absinthe.Middleware.Dataloader | Absinthe.Plugin.defaults()]
  end

  def query(queryable, _params) do
    queryable
  end

  def dataloader() do
    source = Dataloader.Ecto.new(SmoothieApi.Web.Repo, query: &SmoothieApi.Web.Schema.query/2)

    Dataloader.new()
    |> Dataloader.add_source(:db, source)
  end

  def context(ctx) do
    Map.put(ctx, :loader, dataloader())
  end
end
