defmodule AssessmentApi.Web.Model.Classroom do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "classrooms"
  @validated_fields [:id, :name, :owner_id, :teacher_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:name, :string, default: "")
    belongs_to(:teacher, AssessmentApi.Web.Model.Teacher, foreign_key: :teacher_id)
    belongs_to(:owner, AssessmentApi.Web.Model.User, foreign_key: :owner_id)
    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    name: String.t(),
    teacher_id: String.t(),
    owner_id: String.t()
  )
end
