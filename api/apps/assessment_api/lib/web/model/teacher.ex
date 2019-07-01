defmodule AssessmentApi.Web.Model.Teacher do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "teachers"
  @validated_fields [:id, :user_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    belongs_to(:user, AssessmentApi.Web.Model.User, foreign_key: :user_id)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    user_id: String.t()
  )
end
