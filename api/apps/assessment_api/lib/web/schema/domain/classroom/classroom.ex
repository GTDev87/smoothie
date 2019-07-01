defmodule AssessmentApi.Web.Schema.Domain.Classroom do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :classroom do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Classroom, :id))
    end

    field :name, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Classroom, :name))
    end

    field :student_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentClassroom}, %{}}, [{:classroom_id, id}])
        |> on_load(fn loader ->
          student_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentClassroom}, %{}}, [{:classroom_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.student_id end)

          {:ok, student_ids}
        end)
      end)
    end

    field :students, non_null(list_of(non_null(:student))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentClassroom}, %{}}, [{:classroom_id, id}])
        |> on_load(fn loader ->
          student_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentClassroom}, %{}}, [{:classroom_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.student_id end)
          {:ok, student_ids}
        end)
      end)
    end

    field :test_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
        |> on_load(fn loader ->
          test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.test_id end)
          {:ok, test_ids}
        end)
      end)
    end

    field :tests, non_null(list_of(non_null(:test))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
        |> on_load(fn loader ->
          test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.test_id end)
          {:ok, test_ids}
        end)
      end)
    end

    field :test_analytics, non_null(list_of(non_null(:test_analytics))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
        |> on_load(fn loader ->
          classroom_test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, classroom_test_ids}
        end)
      end)
    end

    field :test_analytics_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
        |> on_load(fn loader ->
          classroom_test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.ClassroomTest}, %{}}, [{:classroom_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.test_id end)
          {:ok, classroom_test_ids}
        end)
      end)
    end
  end
end
