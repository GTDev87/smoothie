defmodule AssessmentApi.Web.Schema.Domain.QuestionAnswerKey do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :question_answer_key do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.QuestionAnswerKey, :id))
    end

    field :score, non_null(:float) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.QuestionAnswerKey, :score))
    end

    field :objective_id, :id do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.QuestionAnswerKey, id)
        |> on_load(fn loader ->
          objective_id =
            loader
            |> Dataloader.get(:db, Web.Model.QuestionAnswerKey, id)
            |> Map.get(:objective_id)

          {:ok, objective_id}
        end)
      end)
    end

    field :objective, :objective do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.QuestionAnswerKey, id)
        |> on_load(fn loader ->
          objective_id =
            loader
            |> Dataloader.get(:db, Web.Model.QuestionAnswerKey, id)
            |> Map.get(:objective_id)

          {:ok, objective_id}
        end)
      end)
    end
  end
end
