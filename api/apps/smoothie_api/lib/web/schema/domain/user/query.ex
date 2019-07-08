defmodule SmoothieApi.Web.Schema.Domain.User.Query do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias SmoothieApi.Web

  object :user do
    field :id, non_null(:id) do
      # resolve(Web.Lib.BatchUtils.get_field(Web.Model.User, :id))
      resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :email, non_null(:string) do
      # resolve(Web.Lib.BatchUtils.get_field(Web.Model.User, :email))
      resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :profile, :profile do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Profile}, %{}}, [{:user_id, id}])
        # |> on_load(fn loader ->
        #   profiles =
        #     Dataloader.get(loader, :db, {{:many, Web.Model.Profile}, %{}}, [{:user_id, id}])

        #   profile = (if profiles == [], do: nil, else: List.first(profiles))
        #   {:ok, (if profile == nil, do: nil, else: profile.id)}
        # end)
        {:ok, "nil"}
      end)
    end
  end
end
