defmodule AssessmentApi.Web.Schema.Member.Mutations.StudentTest do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input GiveQuestionToTestInput do
    field(:id, non_null(:id))
    field(:original_id, non_null(:id))
    field(:student_test_id, non_null(:id))
  end

  object :student_test_member_mutations do
    field :give_question_to_test, type: :student_test do
      arg(:student_question, :give_question_to_test_input)

      resolve(&give_question_to_test/2)
    end
  end

  @spec give_question_to_test(
          %{student_question: GiveQuestionToTestInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def give_question_to_test(args, _info) do
    args.student_question.id
    |> AssessmentApi.Web.Model.StudentQuestion.id(query_type: :mutation)
    |> case do
      nil ->
        multi =
          Ecto.Multi.new()
          |> AssessmentApi.Web.Model.StudentQuestion.Multi.insert(
            %AssessmentApi.Web.Model.StudentQuestion{
              id: args.student_question.id,
              original_id: args.student_question.original_id,
              answer: "",
              student_test_id: args.student_question.student_test_id
            }
          )

        AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
        |> case do
          {:ok, _} -> {:ok, args.student_question.student_test_id}
          {:error, changeset} -> {:error, changeset}
        end

      _ ->
        {:ok, args.student_question.student_test_id}
    end
  end
end
