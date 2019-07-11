defmodule SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Projection do
  use Commanded.Projections.Ecto, name: "SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Projection"

  alias SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Event

  def after_update(_event, _metadata, changes), do: App.Notifications.publish_changes(changes)

  project %Event{} = created do
    multi =
      Ecto.Multi.new()
      |> SmoothieApi.Web.Model.User.Multi.insert(%SmoothieApi.Web.Model.User{
        id: created.id,
        email: created.email
      })

    SmoothieApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, created.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
