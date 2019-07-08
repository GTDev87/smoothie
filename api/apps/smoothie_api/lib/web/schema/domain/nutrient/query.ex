defmodule SmoothieApi.Web.Schema.Domain.Nutrient.Query do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias SmoothieApi.Web

  object :nutrient_info do
    field :id, non_null(:id) do
      # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Profile, :id))
      resolve(fn id, _, info -> {:ok, "123"} end)
    end
    field :name, non_null(:id) do
      # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Profile, :id))
      resolve(fn id, _, info -> {:ok, "123"} end)
    end
  end
end
