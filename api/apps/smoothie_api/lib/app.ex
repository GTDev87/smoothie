defmodule SmoothieApi do
  use Application

  # See http://elixir-lang.org/docs/stable/elixir/Application.html
  # for more information on OTP Applications
  def start(_type, _args) do
    import Supervisor.Spec

    # Define workers and child supervisors to be supervised
    children = [
      # Start the Ecto repository
      SmoothieApi.Web.Repo,
      SmoothieApi.Web.WriteRepo,
      # Start the endpoint when the application starts
      SmoothieApi.Web.Endpoint,
      # worker(SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Projection, [], id: :user_aggregate_projector),
      # Start your own worker by calling: SmoothieApi.Web.Worker.start_link(arg1, arg2, arg3)
      # worker(SmoothieApi.Web.Worker, [arg1, arg2, arg3]),
      supervisor(SmoothieApi.Web.Schema.Domain.User.Supervisor, []),
    ]

    # See http://elixir-lang.org/docs/stable/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: SmoothieApi.Supervisor]
    Supervisor.start_link(children, opts)
  end

  # Tell Phoenix to update the endpoint configuration
  # whenever the application is updated.
  def config_change(changed, _new, removed) do
    SmoothieApi.Web.Endpoint.config_change(changed, removed)
    :ok
  end
end
