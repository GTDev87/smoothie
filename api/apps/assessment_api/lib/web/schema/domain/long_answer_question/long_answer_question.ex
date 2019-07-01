defmodule AssessmentApi.Web.Schema.Domain.LongAnswerQuestion do
  use Absinthe.Schema.Notation

  alias AssessmentApi.Web

  object :long_answer_question do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end

    field :question_base_id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end

    field :question_base, non_null(:question_base) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Question, :id))
    end
  end
end
