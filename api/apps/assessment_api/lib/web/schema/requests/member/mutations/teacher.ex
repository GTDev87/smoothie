defmodule AssessmentApi.Web.Schema.Member.Mutations.Teacher do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input ClassroomInput do
    field(:id, non_null(:id))
    field(:name, non_null(:string))
    field(:teacher_id, non_null(:id))
  end

  def_absinthe_input TestInput do
    field(:id, non_null(:id))
    field(:name, non_null(:string))
    field(:description, non_null(:string))
    field(:teacher_id, non_null(:id))
  end

  object :teacher_member_mutations do
    field :add_classroom, type: :teacher do
      arg(:classroom, :classroom_input)

      resolve(&add_classroom/2)
    end

    field :add_test, type: :teacher do
      arg(:test, :test_input)

      resolve(&add_test/2)
    end
  end

  @spec add_classroom(
          %{classroom: ClassroomInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_classroom(args, info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Classroom.Multi.insert(%AssessmentApi.Web.Model.Classroom{
        id: args.classroom.id,
        name: args.classroom.name,
        teacher_id: args.classroom.teacher_id,
        owner_id: info.context.current_user.id
      })

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.classroom.teacher_id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec add_test(
          %{test: TestInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_test(args, info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Test.Multi.insert(%AssessmentApi.Web.Model.Test{
        id: args.test.id,
        name: args.test.name,
        description: args.test.description,
        teacher_id: args.test.teacher_id,
        owner_id: info.context.current_user.id
      })

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutaion, multi)
    |> case do
      {:ok, _} -> {:ok, args.test.teacher_id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
