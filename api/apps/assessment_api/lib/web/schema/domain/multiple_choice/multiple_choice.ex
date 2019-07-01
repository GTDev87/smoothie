defmodule AssessmentApi.Web.Schema.Domain.MultipleChoice do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :multiple_choice do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.MultipleChoice, :id))
    end

    field :choice_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Choice}, %{}}, [{:multiple_choice_id, id}])
        |> on_load(fn loader ->
          multiple_choice_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Choice}, %{}}, [{:multiple_choice_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)

          {:ok, multiple_choice_ids}
        end)
      end)
    end

    field :choices, non_null(list_of(non_null(:choice))) do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Choice}, %{}}, [{:multiple_choice_id, id}])
        |> on_load(fn loader ->
          multiple_choice_ids =
            loader
            |> Dataloader.get(:db, {{:many, Web.Model.Choice}, %{}}, [{:multiple_choice_id, id}])
            |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
            |> Enum.map(fn t -> t.id end)

          {:ok, multiple_choice_ids}
        end)
      end)
    end
  end
end
