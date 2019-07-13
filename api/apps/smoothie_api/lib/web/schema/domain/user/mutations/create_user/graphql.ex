defmodule SmoothieApi.Web.Schema.Domain.User.Mutations.CreateUser.GraphQL do
  use Absinthe.Schema.Notation
  use SmoothieApi.Web.Lib.AbsintheInputUtils

  alias SmoothieApi.Web.Schema.Domain.User.Mutations.CreateUser

  def_absinthe_input UserInput do
    field(:id, non_null(:id))
    field(:email, non_null(:string))
  end

  object :create_user do
    field :create_user, type: :user do
      arg(:user, :user_input)

      resolve(&execute/2)
    end
  end

  @spec execute(%{user: Input.UserInput.t()}, SmoothieApi.Guardian.Context.info()) :: {:error, any()} | {:ok, String}
  def execute(args, _info) do
    uuid = args.user.id

    args
    |> CreateUser.Command.new()
    |> CreateUser.Command.update(args.user)
    |> SmoothieApi.Web.Schema.Router.dispatch(include_aggregate_version: true, consistency: :strong)
    |> case do
      {:ok, version} -> {:ok, uuid}
      :ok -> {:ok, uuid}
      reply -> reply
    end
  end
end
