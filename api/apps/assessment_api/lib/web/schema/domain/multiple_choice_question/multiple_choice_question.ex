defmodule AssessmentApi.Web.Schema.Domain.MultipleChoiceQuestion do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :multiple_choice_question do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end

    field :question_base_id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end

    field :question_base, non_null(:question_base) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end

    field :multiple_choice, non_null(:multiple_choice) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.MultipleChoice}, %{}}, [{:question_id, id}])
        |> on_load(fn loader ->
          [multiple_choice] =
            Dataloader.get(loader, :db, {{:many, Web.Model.MultipleChoice}, %{}}, [{:question_id, id}])
          {:ok, multiple_choice.id}
        end)
      end)
    end
  end
end
