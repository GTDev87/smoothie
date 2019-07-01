defmodule AssessmentApi.Web.Router do
  use AssessmentApi.Web, :router

  pipeline :api do
    plug AssessmentApi.Guardian.AuthPipeline
    # plug AssessmentApi.Web.Context
    # plug Guardian.Plug.VerifyHeader
    plug :accepts, ["json"]
  end

  scope "/" do
    pipe_through :api

    forward "/api", Absinthe.Plug,
      schema: AssessmentApi.Web.Schema

    forward "/graphiql", Absinthe.Plug.GraphiQL,
      schema: AssessmentApi.Web.Schema
  end
end
