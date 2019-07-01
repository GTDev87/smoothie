defmodule AssessmentApi.Web.Schema.Domain.QuestionBase do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  require Logger

  object :question_base do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end

    field :text, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :text))
    end

    field :solution, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :solution))
    end

    field :question_type, non_null(:question_type) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, Web.Model.Question, id)
        |> on_load(fn loader ->
          question_type =
            loader
            |> Dataloader.get(:db, Web.Model.Question, id)
            |> Map.get(:question_type)
            |> Web.Schema.Domain.Question.QuestionType.to_enum()

          {:ok, question_type}
        end)
      end)
    end

    field :question_number, non_null(:integer) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.Question, :id, fn (loader, this_questions) ->
          this_question = this_questions |> List.first
          test_id = this_question.test_id
          Web.Lib.BatchUtils.batch_through_field(loader, test_id, Web.Model.Question, :test_id, fn (loader, questions) ->
            question_index =
              questions
              |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
              |> Enum.map(fn q -> q.id end)
              |> Enum.find_index(fn qid -> qid == this_question.id end)

            {:ok, question_index + 1}
          end)
        end)
      end)
    end

    field :auto_score, non_null(:boolean) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :auto_score))
    end

    field :forced_response, non_null(:boolean) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :forced_response))
    end

    field :stimulus_id, :id do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Stimulus}, %{}}, [{:question_id, id}])
        |> on_load(fn loader ->
          loader
          |> Dataloader.get(:db, {{:many, Web.Model.Stimulus}, %{}}, [{:question_id, id}])
          |> case do
            [] -> {:ok, nil}
            stimuli ->
              [stimulus] = stimuli
              {:ok, stimulus.id}
          end
        end)
      end)
    end

    field :stimulus, :stimulus do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Stimulus}, %{}}, [{:question_id, id}])
        |> on_load(fn loader ->
          loader
          |> Dataloader.get(:db, {{:many, Web.Model.Stimulus}, %{}}, [{:question_id, id}])
          |> case do
            [] -> {:ok, nil}
            stimuli ->
              [stimulus] = stimuli
              {:ok, stimulus.id}
          end
        end)
      end)
    end

    field :answer_key_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.QuestionAnswerKey}, %{}}, [{:question_id, id}])
        |> on_load(fn loader ->
          question_answer_key_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.QuestionAnswerKey}, %{}}, [{:question_id, id}])
            |> Enum.map(fn t -> t.id end)

          {:ok, question_answer_key_ids}
        end)
      end)
    end

    field :answer_keys, non_null(list_of(non_null(:question_answer_key))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.QuestionAnswerKey}, %{}}, [{:question_id, id}])
        |> on_load(fn loader ->
          question_answer_key_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.QuestionAnswerKey}, %{}}, [{:question_id, id}])
            |> Enum.map(fn t -> t.id end)

          {:ok, question_answer_key_ids}
        end)
      end)
    end
  end
end
