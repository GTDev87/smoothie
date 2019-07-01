defmodule AssessmentApi.Web.Model.StudentAnswerKey do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "student_answer_keys"
  @validated_fields [:id, :correct, :original_id, :student_question_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:correct, :boolean, default: false)
    belongs_to(:original, AssessmentApi.Web.Model.QuestionAnswerKey, foreign_key: :original_id)

    belongs_to(
      :student_question,
      AssessmentApi.Web.Model.StudentQuestion,
      foreign_key: :student_question_id
    )

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    correct: boolean,
    student_question_id: String.t(),
    original_id: String.t()
  )
end
