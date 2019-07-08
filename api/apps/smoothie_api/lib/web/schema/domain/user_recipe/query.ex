defmodule SmoothieApi.Web.Schema.Domain.UserRecipe.Query do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias SmoothieApi.Web

  object :user_recipe do
    field :id, non_null(:id) do
      {:ok, "123"}
    end

    field :creator, non_null(:profile) do
      {:ok, "123"}
    end

    field :creator_id, non_null(:id) do
      {:ok, "123"}
    end

    field :num_favorites, non_null(:integer) do
      {:ok, "123"}
    end

    field :name, non_null(:string) do
      {:ok, "123"}
    end

    field :description, non_null(:string) do
      {:ok, "123"}
    end

    field :recipe, non_null(:recipe) do
      {:ok, "123"}
    end

    field :recipe_id, non_null(:id) do
      {:ok, "123"}
    end
  end
end
