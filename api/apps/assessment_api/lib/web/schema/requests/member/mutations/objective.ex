defmodule AssessmentApi.Web.Schema.Member.Mutations.Objective do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input ObjectiveInput do
    field(:id, non_null(:id))
    field(:text, non_null(:string))
  end

  object :objective_member_mutations do
    field :create_update_objective, type: :objective do
      arg(:objective, :objective_input)

      resolve(&create_update_objective/2)
    end
  end

  @spec create_update_objective(
          %{objective: ObjectiveInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def create_update_objective(args, info) do
    objective = AssessmentApi.Web.Model.Objective.id(args.objective.id, query_type: :mutation)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Objective.Multi.upsert(
        objective,
        %AssessmentApi.Web.Model.Objective{
          id: args.objective.id,
          text: args.objective.text,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.objective.id}
      failure -> {:error, "transaction failure #{inspect(failure)}"}
    end
  end
end
