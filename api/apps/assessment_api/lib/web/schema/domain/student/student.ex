defmodule AssessmentApi.Web.Schema.Domain.Student do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  input_object :give_test_to_student_input do
    field(:id, non_null(:id))
    field(:test_id, non_null(:id))
  end

  object :student do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Student, :id))
    end

    field :first_name, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Student, :first_name))
    end

    field :last_name, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Student, :last_name))
    end

    field :grade_id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Student, :grade_id))
    end

    field :grade, non_null(:grade) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Student, :grade_id))
    end

    field :test_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentTest}, %{}}, [{:student_id, id}])
        |> on_load(fn loader ->
          student_test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentTest}, %{}}, [{:student_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, student_test_ids}
        end)
      end)
    end

    field :tests, non_null(list_of(non_null(:student_test))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentTest}, %{}}, [{:student_id, id}])
        |> on_load(fn loader ->
          student_test_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentTest}, %{}}, [{:student_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)

          {:ok, student_test_ids}
        end)
      end)
    end
  end
end
