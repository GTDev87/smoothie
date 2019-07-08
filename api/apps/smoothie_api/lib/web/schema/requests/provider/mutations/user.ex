defmodule SmoothieApi.Web.Schema.Provider.Mutations.User do
  use Absinthe.Schema.Notation
  use SmoothieApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input UserInput do
    field(:id, non_null(:id))
    field(:email, non_null(:string))
  end

  object :user_provider_mutations do
    field :create_user, type: :user do
      arg(:user, :user_input)

      resolve(&create/2)
    end
  end

  @spec create(
          %{user: UserInput.t()},
          SmoothieApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def create(args, _info) do
    multi =
      Ecto.Multi.new()
      |> SmoothieApi.Web.Model.User.Multi.insert(%SmoothieApi.Web.Model.User{
        id: args.user.id,
        email: args.user.email
      })

    SmoothieApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.user.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
