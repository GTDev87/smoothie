defmodule AssessmentApi.Web.Schema.Domain.StudentTest do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :student_test do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.StudentTest, :id))
    end

    field :question_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentQuestion}, %{}}, [{:student_test_id, id}])
        |> on_load(fn loader ->
          student_test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentQuestion}, %{}}, [{:student_test_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, student_test_ids}
        end)
      end)
    end

    field :questions, non_null(list_of(non_null(:student_question))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentQuestion}, %{}}, [{:student_test_id, id}])
        |> on_load(fn loader ->
          student_test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentQuestion}, %{}}, [{:student_test_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, student_test_ids}
        end)
      end)
    end

    field :original_id, non_null(:id) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.StudentTest, id)
        |> on_load(fn loader ->
          student_test =
            Dataloader.get(loader, :db, Web.Model.StudentTest, id)

          {:ok, student_test.original_id}
        end)
      end)
    end

    field :original, non_null(:test) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.StudentTest, id)
        |> on_load(fn loader ->
          student_test =
            Dataloader.get(loader, :db, Web.Model.StudentTest, id)

          {:ok, student_test.original_id}
        end)
      end)
    end
  end
end
