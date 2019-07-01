defmodule AssessmentApi.Web.Schema.Member.Mutations.StudentQuestion do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input GiveAnswerKeyToQuestionInput do
    field(:id, non_null(:id))
    field(:original_id, non_null(:id))
    field(:student_question_id, non_null(:id))
  end

  def_absinthe_input UpdateStudentQuestionInput do
    field(:id, non_null(:id))
    field(:answer, non_null(:string))
  end

  object :student_question_member_mutations do
    field :give_answer_key_to_question, type: :student_question do
      arg(:student_answer_key, :give_answer_key_to_question_input)

      resolve(&give_answer_key_to_question/2)
    end

    field :update_student_question, type: :student_question do
      arg(:student_question, :update_student_question_input)

      resolve(&update_student_question/2)
    end
  end

  @spec give_answer_key_to_question(
          %{student_answer_key: GiveAnswerKeyToQuestionInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def give_answer_key_to_question(args, _info) do
    args.student_answer_key.id
    |> AssessmentApi.Web.Model.StudentAnswerKey.id(query_type: :mutation)
    |> case do
      nil ->
        multi =
          Ecto.Multi.new()
          |> AssessmentApi.Web.Model.StudentAnswerKey.Multi.insert(
            %AssessmentApi.Web.Model.StudentAnswerKey{
              id: args.student_answer_key.id,
              original_id: args.student_answer_key.original_id,
              student_question_id: args.student_answer_key.student_question_id
            }
          )

        AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
        |> case do
          {:ok, _} -> {:ok, args.student_answer_key.student_question_id}
          {:error, changeset} -> {:error, changeset}
        end

      _ ->
        {:ok, args.student_answer_key.student_question_id}
    end
  end

  @spec update_student_question(
          %{student_question: UpdateStudentQuestionInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_student_question(args, _info) do
    student_question =
      AssessmentApi.Web.Model.StudentQuestion.id(args.student_question.id, query_type: :mutation)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.StudentQuestion.Multi.update(
        student_question,
        %{
          id: args.student_question.id,
          answer: args.student_question.answer
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.student_question.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
