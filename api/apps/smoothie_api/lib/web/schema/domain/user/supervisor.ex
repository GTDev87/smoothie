defmodule SmoothieApi.Web.Schema.Domain.User.Supervisor do
  use Supervisor

  alias SmoothieApi.Web.Schema.Domain.User.Projector
  
  def start_link do
    Supervisor.start_link(__MODULE__, [], name: __MODULE__)
  end

  def init(_arg) do
    Supervisor.init([
      Projector
    ], strategy: :one_for_one)
  end
end
