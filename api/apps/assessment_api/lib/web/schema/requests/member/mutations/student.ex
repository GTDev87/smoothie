defmodule AssessmentApi.Web.Schema.Member.Mutations.Student do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input StudentInput do
    field(:id, non_null(:id))
    field(:grade_id, non_null(:id))
    field(:first_name, non_null(:string))
    field(:last_name, non_null(:string))
  end

  def_absinthe_input StudentTestInput do
    field(:id, non_null(:id))
    field(:original_id, non_null(:id))
    field(:student_id, non_null(:id))
  end

  object :student_member_mutations do
    field :update_student, type: :student do
      arg(:student, :student_input)

      resolve(&update_student/2)
    end

    field :give_test_to_student, type: :student do
      arg(:student_test, :student_test_input)

      resolve(&give_test_to_student/2)
    end
  end

  @spec update_student(
          %{student: StudentInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_student(args, info) do
    student = AssessmentApi.Web.Model.Student.id(args.student.id, query_type: :mutation)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Student.Multi.upsert(
        student,
        %AssessmentApi.Web.Model.Student{
          id: args.student.id,
          first_name: args.student.first_name,
          last_name: args.student.last_name,
          grade_id: args.student.grade_id,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.student.id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec give_test_to_student(
          %{student_test: StudentTestInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def give_test_to_student(args, _info) do
    args.student_test.id
    |> AssessmentApi.Web.Model.StudentTest.id(query_type: :mutation)
    |> case do
      nil ->
        multi =
          Ecto.Multi.new()
          |> AssessmentApi.Web.Model.StudentTest.Multi.insert(
            %AssessmentApi.Web.Model.StudentTest{
              id: args.student_test.id,
              original_id: args.student_test.original_id,
              student_id: args.student_test.student_id
            }
          )

        AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
        |> case do
          {:ok, _} -> {:ok, args.student_test.student_id}
          {:error, changeset} -> {:error, changeset}
        end

      _ ->
        {:ok, args.student_test.student_id}
    end
  end
end
