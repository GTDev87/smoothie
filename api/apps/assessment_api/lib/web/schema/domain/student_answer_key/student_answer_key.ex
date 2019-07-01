defmodule AssessmentApi.Web.Schema.Domain.StudentAnswerKey do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :student_answer_key do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.StudentAnswerKey, :id))
    end

    field :correct, non_null(:boolean) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.StudentAnswerKey, :correct))
    end

    field :original_id, non_null(:id) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.StudentAnswerKey, id)
        |> on_load(fn loader ->
          student_answer_keys =
            Dataloader.get(loader, :db, Web.Model.StudentAnswerKey, id)

          {:ok, student_answer_keys.original_id}
        end)
      end)
    end

    field :original, non_null(:question_answer_key) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.StudentAnswerKey, id)
        |> on_load(fn loader ->
          student_answer_keys =
            Dataloader.get(loader, :db, Web.Model.StudentAnswerKey, id)

          {:ok, student_answer_keys.original_id}
        end)
      end)
    end
  end
end
