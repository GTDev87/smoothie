defmodule AssessmentApi.Web.Model.StudentClassroom do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "student_classrooms"
  @validated_fields [:id, :classroom_id, :student_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    belongs_to(:classroom, AssessmentApi.Web.Model.Classroom, foreign_key: :classroom_id)
    belongs_to(:student, AssessmentApi.Web.Model.Student, foreign_key: :student_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    classroom_id: String.t(),
    student_id: String.t()
  )
end
