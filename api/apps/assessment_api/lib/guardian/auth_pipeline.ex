defmodule AssessmentApi.Guardian.AuthPipeline do
  @claims %{typ: "access"}

  use Guardian.Plug.Pipeline,
    otp_app: :assessment_api,
    module: AssessmentApi.Guardian,
    error_handler: AssessmentApi.Guardian.AuthErrorHandler

  # TODO REMEMBER TO TURN THIS ON WHEN GOING TO PROD
  plug(Guardian.Plug.VerifySession, claims: @claims)
  plug(Guardian.Plug.VerifyHeader, claims: @claims, realm: "Bearer")
  plug(Guardian.Plug.EnsureAuthenticated)
  plug(Guardian.Plug.LoadResource, allow_blank: false)
  plug(AssessmentApi.Guardian.Context)
end
