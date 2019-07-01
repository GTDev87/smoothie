defmodule AssessmentApi.Web.Model.Grade do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "grades"
  @validated_fields [:id, :name]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:name, :string, default: "")

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    name: String.t()
  )
end
