defmodule AssessmentApi.Web.Schema.Domain.Test do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :test do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Test, :id))
    end

    field :name, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Test, :name))
    end

    field :notes, :string do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Test, :notes))
    end

    field :description, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Test, :description))
    end

    field :question_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Question}, %{}}, [{:test_id, id}])
        |> on_load(fn loader ->
          question_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Question}, %{}}, [{:test_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)

          # TODO this is for an inefficiency with question loading
          loader
          |> Dataloader.load_many(:db, Web.Model.Question, question_ids)
          |> on_load(fn loader ->
            question_ids =
              loader
              |> Dataloader.get_many(:db, Web.Model.Question, question_ids)
              |> Enum.map(fn t -> t.id end)

            {:ok, question_ids}
          end)
        end)
      end)
    end

    field :questions, non_null(list_of(non_null(:question))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Question}, %{}}, [{:test_id, id}])
        |> on_load(fn loader ->
          question_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Question}, %{}}, [{:test_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)

          # TODO this is for an inefficiency with question loading
          loader
          |> Dataloader.load_many(:db, Web.Model.Question, question_ids)
          |> on_load(fn loader ->
            question_ids =
              loader
              |> Dataloader.get_many(:db, Web.Model.Question, question_ids)
              |> Enum.map(fn t -> t.id end)

            {:ok, question_ids}
          end)
        end)
      end)
    end

    field :num_questions, non_null(:integer) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.Test.Helper.get_num_questions(info.context.loader, id, fn (loader, num_questions) ->
          {:ok, num_questions}
        end)
      end)
    end

    field :objective_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.TestObjective}, %{}}, [{:test_id, id}])
        |> on_load(fn loader ->
          objective_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.TestObjective}, %{}}, [{:test_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.objective_id end)
          {:ok, objective_ids}
        end)
      end)
    end

    field :objectives, non_null(list_of(non_null(:objective))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.TestObjective}, %{}}, [{:test_id, id}])
        |> on_load(fn loader ->
          objective_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.TestObjective}, %{}}, [{:test_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.objective_id end)
          {:ok, objective_ids}
        end)
      end)
    end
  end
end
