defmodule AssessmentApi.Web.Schema.Member.Mutations.StudentAnswerKey do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input UpdateStudentAnswerKeyInput do
    field(:id, non_null(:id))
    field(:correct, non_null(:boolean))
  end

  object :student_answer_key_member_mutations do
    field :update_student_answer_key, type: :student_answer_key do
      arg(:student_answer_key, :update_student_answer_key_input)

      resolve(&update_student_answer_key/2)
    end
  end

  @spec update_student_answer_key(
          %{student_answer_key: UpdateStudentAnswerKeyInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_student_answer_key(args, _info) do
    student_answer_key =
      AssessmentApi.Web.Model.StudentAnswerKey.id(
        args.student_answer_key.id,
        query_type: :mutation
      )

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.StudentAnswerKey.Multi.update(
        student_answer_key,
        %{
          id: args.student_answer_key.id,
          correct: args.student_answer_key.correct
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.student_answer_key.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
