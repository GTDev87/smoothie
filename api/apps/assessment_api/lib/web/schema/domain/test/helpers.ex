defmodule AssessmentApi.Web.Schema.Domain.Test.Helper do
  alias AssessmentApi.Web

  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  #id test id

  def get_answer_keys(loader, id, callback) do
    loader
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

        Web.Lib.BatchUtils.batch_through_field(loader, question_ids, Web.Model.QuestionAnswerKey, :question_id, fn (loader, answer_keys) ->
          callback.(loader, answer_keys)
        end)
      end)
    end)
  end

  def get_num_questions(loader, id, callback) do
    get_answer_keys(loader, id, fn (loader, answer_keys) ->
      callback.(loader, length(answer_keys))
    end)
  end
end
