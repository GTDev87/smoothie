defmodule AssessmentApi.Web.Model.Choice do
  use AssessmentApi.Web, :model
  use AssessmentApi.Web.Lib.EctoTypeUtils
  @model "choices"
  @validated_fields [:id, :text, :multiple_choice_id]

  @primary_key {:id, :binary_id, autogenerate: false}
  @foreign_key_type Ecto.UUID
  schema @model do
    # field :id, Ecto.UUID
    field(:text, :string)

    belongs_to(
      :multiple_choice,
      AssessmentApi.Web.Model.MultipleChoice,
      foreign_key: :multiple_choice_id
    )

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    text: String.t(),
    multiple_choice_id: String.t()
  )
end
