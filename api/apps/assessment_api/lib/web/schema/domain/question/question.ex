defmodule AssessmentApi.Web.Schema.Domain.Question do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  alias AssessmentApi.Web

  typed_enum QuestionType do
    value(:long_answer, 0)
    value(:multiple_choice, 1)
    value(:true_false, 2)
    value(:fill_in_the_blank, 3)
  end

  union :question do
    types([
      :multiple_choice_question,
      :long_answer_question,
      :true_false_question,
      :fill_in_the_blank_question
    ])

    resolve_type(fn id, info ->
      # TODO insanely inefficient n+1 because no batching able for union
      # require Logger
      # Logger.debug "question resolve_type"
      # Logger.debug "info.context.loader = #{inspect info.context.loader}"
      try do
        # kinda gross
        Dataloader.get(info.context.loader, :db, Web.Model.Question, id)
      rescue
        e -> nil
      end
      |> case do
        nil -> Web.Model.Question.id(id)
        question -> question
      end
      |> Map.get(:question_type, nil)
      |> Web.Schema.Domain.Question.QuestionType.to_enum()
      |> case do
        :multiple_choice -> :multiple_choice_question
        :long_answer -> :long_answer_question
        :true_false -> :true_false_question
        :fill_in_the_blank -> :fill_in_the_blank_question
      end
    end)
  end
end
