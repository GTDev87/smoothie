defmodule SmoothieApi.Web.Schema.Domain.Ingredient.Query do
  use Absinthe.Schema.Notation
  use SmoothieApi.Web.Lib.AbsintheInputUtils

  alias SmoothieApi.Web

  typed_enum IngredientType do
    value(:nbd, 0)
    value(:nbd_branded, 1)
    value(:app, 2)
  end

  union :ingredient do
    types([
      :nbd_ingredient,
      :nbd_branded_ingredient,
      :app_ingredient,
    ])

    resolve_type(fn id, info ->
        # TODO insanely inefficient n+1 because no batching able for union
        # require Logger
        # Logger.debug "question resolve_type"
        # Logger.debug "info.context.loader = #{inspect info.context.loader}"




        #   try do
        #     # kinda gross
        #     Dataloader.get(info.context.loader, :db, Web.Model.Question, id)
        #   rescue
        #     e -> nil
        #   end
        #   |> case do
        #     nil -> Web.Model.Question.id(id)
        #     question -> question
        #   end
        #   |> Map.get(:question_type, nil)
        #   |> Web.Schema.Domain.Question.QuestionType.to_enum()
        #   |> case do
        #     :multiple_choice -> :multiple_choice_question
        #     :long_answer -> :long_answer_question
        #     :true_false -> :true_false_question
        #     :fill_in_the_blank -> :fill_in_the_blank_question
        #   end
        :nbd_ingredient
    end)
  end
end
