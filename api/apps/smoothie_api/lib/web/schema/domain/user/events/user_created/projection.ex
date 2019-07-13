defmodule SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Projection do
  alias SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Event

  def execute(multi, %Event{} = created) do
    SmoothieApi.Web.Model.User.Multi.insert(multi, %SmoothieApi.Web.Model.User{
      id: created.id,
      email: created.email
    })
  end
end
