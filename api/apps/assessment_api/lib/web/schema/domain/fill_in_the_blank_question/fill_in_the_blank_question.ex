defmodule AssessmentApi.Web.Schema.Domain.FillInTheBlankQuestion do
  use Absinthe.Schema.Notation

  alias AssessmentApi.Web

  object :fill_in_the_blank_question do
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
