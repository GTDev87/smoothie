defmodule AssessmentApi.Web.Model.StudentTest do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "student_tests"
  @validated_fields [:id, :original_id, :student_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    belongs_to(:original, AssessmentApi.Web.Model.Test, foreign_key: :original_id)
    belongs_to(:student, AssessmentApi.Web.Model.Student, foreign_key: :student_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    original_id: String.t(),
    student_id: String.t()
  )
end
