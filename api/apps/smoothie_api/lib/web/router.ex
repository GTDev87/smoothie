defmodule SmoothieApi.Web.Router do
  use SmoothieApi.Web, :router

  pipeline :api do
    plug SmoothieApi.Guardian.AuthPipeline
    # plug SmoothieApi.Web.Context
    # plug Guardian.Plug.VerifyHeader
    plug :accepts, ["json"]
  end

  scope "/" do
    pipe_through :api

    forward "/api", Absinthe.Plug,
      schema: SmoothieApi.Web.Schema

    forward "/graphiql", Absinthe.Plug.GraphiQL,
      schema: SmoothieApi.Web.Schema,
      socket: SmoothieApi.Web.UserSocket
  end
end
