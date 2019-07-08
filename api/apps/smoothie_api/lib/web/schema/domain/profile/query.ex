defmodule SmoothieApi.Web.Schema.Domain.Profile.Query do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias SmoothieApi.Web

  object :profile do
    field :id, non_null(:id) do
      # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Profile, :id))
      resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :nickname, non_null(:id) do
      # resolve(Web.Lib.BatchUtils.get_field(Web.Model.Teacher, :name))
      resolve(fn id, _, info -> {:ok, "123"} end)
    end

    field :created_recipe_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end

    field :created_recipes, non_null(list_of(non_null(:user_recipe))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end

    





    field :history_recipe_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end

    field :history_recipes, non_null(list_of(non_null(:user_recipe))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end


    

    field :favorite_recipes, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end

    field :favorite_recipe_ids, non_null(list_of(non_null(:user_recipe))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end



    field :draft_recipes, non_null(list_of(non_null(:draft_recipe))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end

    field :draft_recipe_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        {:ok, []}
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
      end)
    end

    field :draft_recipe_ids, non_null(list_of(non_null(:draft_recipe))) do
      resolve(fn id, _, info ->
        # info.context.loader
        # |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        # |> on_load(fn loader ->
        #   test_ids =
        #     loader
        #     |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        #     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
        #     |> Enum.map(fn t -> t.id end)
        #   {:ok, test_ids}
        # end)
        {:ok, []}
      end)
    end

    field :num_favorites, non_null(:integer) do
      resolve(fn id, _, info ->
        {:ok, 0}
      end)
    end


  end
end
