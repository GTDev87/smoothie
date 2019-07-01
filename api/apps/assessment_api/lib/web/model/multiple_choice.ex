defmodule AssessmentApi.Web.Model.MultipleChoice do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "multiple_choices"
  @validated_fields [:id, :question_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    belongs_to(:question, AssessmentApi.Web.Model.Question, foreign_key: :question_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    question_id: String.t()
  )
end
