defmodule AssessmentApi.Web.Schema.Domain.Teacher do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :teacher do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Teacher, :id))
    end

    field :test_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        |> on_load(fn loader ->
          test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, test_ids}
        end)
      end)
    end

    field :tests, non_null(list_of(non_null(:test))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
        |> on_load(fn loader ->
          test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Test}, %{}}, [{:teacher_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, test_ids}
        end)
      end)
    end

    field :classroom_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Classroom}, %{}}, [{:teacher_id, id}])
        |> on_load(fn loader ->
          classroom_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Classroom}, %{}}, [{:teacher_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, classroom_ids}
        end)
      end)
    end

    field :classrooms, non_null(list_of(non_null(:classroom))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Classroom}, %{}}, [{:teacher_id, id}])
        |> on_load(fn loader ->
          classroom_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Classroom}, %{}}, [{:teacher_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, classroom_ids}
        end)
      end)
    end
  end
end
