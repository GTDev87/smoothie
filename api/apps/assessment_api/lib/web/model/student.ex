defmodule AssessmentApi.Web.Model.Student do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "students"
  @validated_fields [:id, :first_name, :last_name, :owner_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:first_name, :string, default: "")
    field(:last_name, :string, default: "")
    belongs_to(:grade, AssessmentApi.Web.Model.Grade, foreign_key: :grade_id)
    belongs_to(:owner, AssessmentApi.Web.Model.User, foreign_key: :owner_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    first_name: String.t(),
    last_name: String.t(),
    grade_id: String.t(),
    owner_id: String.t()
  )
end
