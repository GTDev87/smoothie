defmodule AssessmentApi.Web.Schema.Domain.Choice do
  use Absinthe.Schema.Notation
  alias AssessmentApi.Web

  object :choice do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Choice, :id))
    end

    field :text, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Choice, :text))
    end
  end
end
