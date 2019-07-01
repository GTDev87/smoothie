defmodule AssessmentApi.Web.Schema.Domain.StudentQuestion do
  use Absinthe.Schema.Notation

  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :student_question do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.StudentQuestion, :id))
    end

    field :answer, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.StudentQuestion, :answer))
    end

    field :original_id, non_null(:id) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.StudentQuestion, id)
        |> on_load(fn loader ->
          studentQuestion =
            Dataloader.get(loader, :db, Web.Model.StudentQuestion, id)

          # TODO This is to fix an inefficiency with question union
          Web.Lib.BatchUtils.get_field_with_loader(loader, Web.Model.Question, studentQuestion.original_id, :id)
        end)
      end)
    end

    field :original, non_null(:question) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.StudentQuestion, id)
        |> on_load(fn loader ->
          studentQuestion =
            Dataloader.get(loader, :db, Web.Model.StudentQuestion, id)

          # TODO This is to fix an inefficiency with question union
          Web.Lib.BatchUtils.get_field_with_loader(loader, Web.Model.Question, studentQuestion.original_id, :id)
        end)
      end)
    end

    field :answer_key_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentAnswerKey}, %{}}, [{:student_question_id, id}])
        |> on_load(fn loader ->
          student_answer_key_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentAnswerKey}, %{}}, [{:student_question_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, student_answer_key_ids}
        end)
      end)
    end

    field :answer_keys, non_null(list_of(non_null(:student_answer_key))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.StudentAnswerKey}, %{}}, [{:student_question_id, id}])
        |> on_load(fn loader ->
          student_answer_key_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.StudentAnswerKey}, %{}}, [{:student_question_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)
          {:ok, student_answer_key_ids}
        end)
      end)
    end
  end
end
