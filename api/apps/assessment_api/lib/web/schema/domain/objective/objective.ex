defmodule AssessmentApi.Web.Schema.Domain.Objective do
  use Absinthe.Schema.Notation

  alias AssessmentApi.Web

  object :objective do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Objective, :id))
    end

    field :text, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Objective, :text))
    end
  end
end
