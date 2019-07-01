defmodule AssessmentApi.Web.Model.Stimulus do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "stimuli"
  @validated_fields [:id, :text, :question_id, :owner_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:text, :string, default: "")
    belongs_to(:question, AssessmentApi.Web.Model.Question, foreign_key: :question_id)
    belongs_to(:owner, AssessmentApi.Web.Model.User, foreign_key: :owner_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    text: String.t(),
    question_id: String.t(),
    owner_id: String.t()
  )
end
