defmodule AssessmentApi.Web.Schema.Member.Mutations.MultipleChoiceQuestion do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input ChoiceInput do
    field(:id, non_null(:id))
    field(:question_id, non_null(:id))
    field(:text, non_null(:string))
  end

  object :multiple_choice_question_member_mutations do
    field :add_choice, type: :multiple_choice_question do
      arg(:choice, :choice_input)

      resolve(&add_choice/2)
    end
  end

  @spec add_choice(
          %{choice: ChoiceInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_choice(args, _info) do
    multiple_choice =
      AssessmentApi.Web.Model.MultipleChoice.question_id(
        args.choice.question_id,
        query_type: :mutation
      )

    multiple_choice_id =
      case multiple_choice do
        nil -> UUID.uuid4()
        _ -> multiple_choice.id
      end

    multi =
      case multiple_choice do
        nil ->
          Ecto.Multi.new()
          |> AssessmentApi.Web.Model.MultipleChoice.Multi.insert(
            %AssessmentApi.Web.Model.MultipleChoice{
              id: multiple_choice_id,
              question_id: args.choice.question_id
            }
          )

        _ ->
          Ecto.Multi.new()
      end
      |> AssessmentApi.Web.Model.Choice.Multi.insert(%AssessmentApi.Web.Model.Choice{
        id: args.choice.id,
        text: args.choice.text,
        multiple_choice_id: multiple_choice_id
      })

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.choice.question_id}
      failure -> {:error, "transaction failure #{inspect(failure)}"}
    end
  end
end
