defmodule SmoothieApi.Web.Model.User do
  use SmoothieApi.Web, :model
  use SmoothieApi.Web.Lib.EctoTypeUtils
  @model "users"
  @validated_fields [:id, :email]

  @primary_key {:id, :binary_id, autogenerate: false}
  schema @model do
    # field :id, Ecto.UUID
    field(:email, :string)

    timestamps()
  end

  def_ecto_types(
    @model,
    id: String.t(),
    email: String.t()
  )
end
