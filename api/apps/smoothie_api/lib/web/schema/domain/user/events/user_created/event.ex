defmodule SmoothieApi.Web.Schema.Domain.User.Events.UserCreated.Event do
    @derive [Poison.Encoder]
    defstruct [
        id: "",
        email: "",
    ]
end