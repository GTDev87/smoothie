defmodule AssessmentApi.Web.Model.Question do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "questions"

  @validated_fields [
    :id,
    :text,
    :solution,
    :auto_score,
    :forced_response,
    :order,
    :owner_id,
    :test_id
  ]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:text, :string, default: "")
    field(:solution, :string, default: "")
    field(:auto_score, :boolean, default: false)
    field(:forced_response, :boolean, default: false)
    field(:order, :integer, default: 0)
    field(:question_type, :integer, default: 0)

    belongs_to(:test, AssessmentApi.Web.Model.Test, foreign_key: :test_id)
    belongs_to(:owner, AssessmentApi.Web.Model.User, foreign_key: :owner_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    text: String.t(),
    solution: String.t(),
    auto_score: boolean,
    forced_response: boolean,
    order: integer,
    question_type: integer,
    test_id: String.t(),
    owner_id: String.t()
  )
end
