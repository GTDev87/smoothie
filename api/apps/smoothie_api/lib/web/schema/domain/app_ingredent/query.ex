defmodule SmoothieApi.Web.Schema.Domain.AppIngredient.Query do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias SmoothieApi.Web

  object :app_ingredient do
    field :id, non_null(:id) do
        # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Profile, :id))
        resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :name, non_null(:string) do
        # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Profile, :id))
        resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :description, non_null(:string) do
        # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Profile, :id))
        resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :nutritional_info, non_null(:nutritional_info) do
        resolve(fn id, _, info -> {:ok, "123"} end)
    end
  end
end