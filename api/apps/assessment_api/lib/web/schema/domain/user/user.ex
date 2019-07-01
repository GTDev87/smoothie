defmodule AssessmentApi.Web.Schema.Domain.User do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  object :user do
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.User, :id))
    end

    field :email, non_null(:string) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.User, :email))
    end

    field :teacher_id, :id do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Teacher}, %{}}, [{:user_id, id}])
        |> on_load(fn loader ->
          teachers =
            Dataloader.get(loader, :db, {{:many, Web.Model.Teacher}, %{}}, [{:user_id, id}])

          teacher = (if teachers == [], do: nil, else: List.first(teachers))
          {:ok, (if teacher == nil, do: nil, else: teacher.id)}
        end)
      end)
    end

    field :teacher, :teacher do
      resolve(fn id, _, info ->
        info.context.loader
        |> Dataloader.load(:db, {{:many, Web.Model.Teacher}, %{}}, [{:user_id, id}])
        |> on_load(fn loader ->
          teachers =
            Dataloader.get(loader, :db, {{:many, Web.Model.Teacher}, %{}}, [{:user_id, id}])

          teacher = (if teachers == [], do: nil, else: List.first(teachers))
          {:ok, (if teacher == nil, do: nil, else: teacher.id)}
        end)
      end)
    end
  end
end
