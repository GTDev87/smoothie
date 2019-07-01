defmodule AssessmentApi.Web.Schema.Domain.Grade do
  use Absinthe.Schema.Notation

  alias AssessmentApi.Web

  object :grade do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Grade, :id))
    end

    field :name, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.Grade, :name))
    end
  end
end
