defmodule AssessmentApi.Web.Schema.Member.Mutations.QuestionAnswerKey do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input UpdateQuestionAnswerKeyInput do
    field(:id, non_null(:id))
    field(:score, non_null(:float))
    field(:objective_id, :id)
  end

  object :question_answer_key_member_mutations do
    field :update_question_answer_key, type: :question_answer_key do
      arg(:question_answer_key, :update_question_answer_key_input)

      resolve(&update_question_answer_key/2)
    end
  end

  @spec update_question_answer_key(
          %{question_answer_key: UpdateQuestionAnswerKeyInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_question_answer_key(args, info) do
    answer_key =
      AssessmentApi.Web.Model.QuestionAnswerKey.id(
        args.question_answer_key.id,
        query_type: :mutation
      )

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.QuestionAnswerKey.Multi.update(
        answer_key,
        %{
          id: args.question_answer_key.id,
          score: args.question_answer_key.score,
          objective_id: args.question_answer_key.objective_id,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.question_answer_key.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
