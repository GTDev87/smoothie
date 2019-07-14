defmodule SmoothieApi.Web.Schema.Domain.User.Projector do
  use Commanded.Projections.Ecto,
    name: "SmoothieApi.Web.Schema.Domain.User.Projector",
    consistency: :strong


  alias SmoothieApi.Web.Schema.Domain.User.Events.{
    UserCreated
  }

  project(%UserCreated.Event{} = event, fn multi ->
    UserCreated.Projection.execute(multi, event)
  end)
end
