defmodule AssessmentApi.Web.Model.ClassroomTest do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "classroom_tests"
  @validated_fields [:id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    belongs_to(:classroom, AssessmentApi.Web.Model.Classroom, foreign_key: :classroom_id)
    belongs_to(:test, AssessmentApi.Web.Model.Test, foreign_key: :test_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    classroom_id: String.t(),
    test_id: String.t()
  )
end
