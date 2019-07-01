defmodule AssessmentApi.Web.Model.QuestionAnswerKey do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "question_answer_keys"
  @validated_fields [:id, :score, :question_id, :owner_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:score, :float, default: 1.0)

    belongs_to(:objective, AssessmentApi.Web.Model.Objective, foreign_key: :objective_id)

    belongs_to(:question, AssessmentApi.Web.Model.Question, foreign_key: :question_id)
    belongs_to(:owner, AssessmentApi.Web.Model.User, foreign_key: :owner_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    score: float,
    objective_id: String.t() | nil,
    question_id: String.t(),
    owner_id: String.t()
  )
end
