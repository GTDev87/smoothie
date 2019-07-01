defmodule AssessmentApi.Web.Model.TestObjective do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "test_objectives"
  @validated_fields [:id, :objective_id, :test_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    belongs_to(:test, AssessmentApi.Web.Model.Test, foreign_key: :test_id)
    belongs_to(:objective, AssessmentApi.Web.Model.Objective, foreign_key: :objective_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    test_id: String.t(),
    objective_id: String.t()
  )
end
