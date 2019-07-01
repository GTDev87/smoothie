defmodule AssessmentApi.Web.Model.StudentQuestion do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "student_questions"
  @validated_fields [:id, :answer, :original_id, :student_test_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:answer, :string, default: "")
    belongs_to(:original, AssessmentApi.Web.Model.Test, foreign_key: :original_id)
    belongs_to(:student_test, AssessmentApi.Web.Model.StudentTest, foreign_key: :student_test_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    answer: String.t(),
    original_id: String.t(),
    student_test_id: String.t()
  )
end
