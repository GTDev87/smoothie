defmodule SmoothieApi.Web.Schema.Router do
  use Commanded.Commands.Router

  alias SmoothieApi.Web.Schema.Domain.User
  

  identify User.Aggregate, by: :id, prefix: "user-"

  dispatch [
      User.Mutations.CreateUser.Command,
  ], to: User.Aggregate
end