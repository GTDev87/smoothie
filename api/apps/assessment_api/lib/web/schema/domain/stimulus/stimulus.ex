defmodule AssessmentApi.Web.Schema.Domain.Stimulus do
  use Absinthe.Schema.Notation

  alias AssessmentApi.Web

  object :stimulus do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Stimulus, :id))
    end

    field :text, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Stimulus, :text))
    end
  end
end
