defmodule AssessmentApi.Web.Model.Objective do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "objectives"
  @validated_fields [:id, :text, :owner_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:text, :string, default: "")
    belongs_to(:owner, AssessmentApi.Web.Model.User, foreign_key: :owner_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    text: String.t(),
    owner_id: String.t()
  )
end
