defmodule SmoothieApi.Guardian.AuthPipeline do
  @claims %{typ: "access"}

  use Guardian.Plug.Pipeline,
    otp_app: :smoothie_api,
    module: SmoothieApi.Guardian,
    error_handler: SmoothieApi.Guardian.AuthErrorHandler

  # TODO REMEMBER TO TURN THIS ON WHEN GOING TO PROD
  plug(Guardian.Plug.VerifySession, claims: @claims)
  plug(Guardian.Plug.VerifyHeader, claims: @claims, realm: "Bearer")
  plug(Guardian.Plug.EnsureAuthenticated)
  plug(Guardian.Plug.LoadResource, allow_blank: false)
  plug(SmoothieApi.Guardian.Context)
end
