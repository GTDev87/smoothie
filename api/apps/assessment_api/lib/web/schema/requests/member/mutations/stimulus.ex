defmodule AssessmentApi.Web.Schema.Member.Mutations.Stimulus do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input UpdateStimulsInput do
    field(:id, non_null(:id))
    field(:text, non_null(:id))
  end

  object :stimulus_member_mutations do
    field :update_stimulus, type: :stimulus do
      arg(:stimulus, :update_stimuls_input)

      resolve(&update_stimulus/2)
    end
  end

  @spec update_stimulus(
          %{stimulus: UpdateStimulsInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_stimulus(args, info) do
    stimulus = AssessmentApi.Web.Model.Stimulus.id(args.stimulus.id, query_type: :mutation)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Stimulus.Multi.update(
        stimulus,
        %{
          id: args.stimulus.id,
          text: args.stimulus.text,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.stimulus.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
