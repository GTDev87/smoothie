# This file is responsible for configuring your application
# and its dependencies with the aid of the Mix.Config module.
#
# This configuration file is loaded before any dependency and
# is restricted to this project.
use Mix.Config

# General application configuration
config :smoothie_api,
  ecto_repos: [SmoothieApi.Web.Repo, SmoothieApi.Web.WriteRepo]


config :commanded,
  event_store_adapter: Commanded.EventStore.Adapters.EventStore

config :commanded_ecto_projections,
  repo: SmoothieApi.Web.Repo

# Configures the endpoint
config :smoothie_api, SmoothieApi.Web.Endpoint,
  url: [host: "localhost"],
  secret_key_base: "q+YJWIzmULVdW3+l8Pf4EOh1dSgEmg4CuNd8GzMRsS/SzDF6F5rckfjAn4Q0jKK1",
  render_errors: [view: SmoothieApi.Web.ErrorView, accepts: ~w(html json)],
  pubsub: [name: SmoothieApi.PubSub, adapter: Phoenix.PubSub.PG2]

# Configures Elixir's Logger
config :logger, :console,
  format: "$time $metadata[$level] $message\n",
  metadata: [:request_id]

# Guardian configuration
config :smoothie_api, SmoothieApi.Guardian,
  allowed_algos: ["HS512"], # optional
  verify_module: Guardian.JWT,  # optional
  issuer: "smoothie_api", #maybe make issuer same as other repo
  ttl: { 30, :days },
  allowed_drift: 2000,
  verify_issuer: true, # optional
  # generated using: JOSE.JWK.generate_key({:oct, 16}) |> JOSE.JWK.to_map |> elem(1)
  secret_key: System.get_env("GUARDIAN_SECRET") || %{"k" => "CMiIxqWkh_Vz_2RNsy11Nw", "kty" => "oct"}#,

# Import environment specific config. This must remain at the bottom
# of this file so it overrides the configuration defined above.
import_config "#{Mix.env}.exs"
