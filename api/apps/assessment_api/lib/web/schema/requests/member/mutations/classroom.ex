defmodule AssessmentApi.Web.Schema.Member.Mutations.Classroom do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input AddStudentInput do
    field(:id, non_null(:id))
    field(:classroom_id, non_null(:id))
  end

  def_absinthe_input UpdateClassroomInput do
    field(:id, non_null(:id))
    field(:name, non_null(:string))
  end

  def_absinthe_input GiveTestToClassroomInput do
    field(:test_id, non_null(:id))
    field(:classroom_id, non_null(:id))
  end

  object :classroom_member_mutations do
    field :add_student, type: :classroom do
      arg(:student, :add_student_input)

      resolve(&add_student/2)
    end

    field :update_classroom, type: :classroom do
      arg(:classroom, :update_classroom_input)

      resolve(&update_classroom/2)
    end

    field :give_test_to_classroom, type: :classroom do
      arg(:classroom_test, :give_test_to_classroom_input)

      resolve(&give_test_to_classroom/2)
    end
  end

  @spec add_student(%{student: AddStudentInput.t()}, AssessmentApi.Guardian.Context.info()) ::
          {:error, any()} | {:ok, String}
  def add_student(args, info) do
    # on creation just choose first grade
    grade_id =
      AssessmentApi.Web.Model.Grade.all([])
      |> Map.values()
      |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
      |> Enum.map(fn g -> g.id end)
      |> List.first()

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Student.Multi.insert(%AssessmentApi.Web.Model.Student{
        id: args.student.id,
        owner_id: info.context.current_user.id,
        grade_id: grade_id
      })
      |> AssessmentApi.Web.Model.StudentClassroom.Multi.insert(
        %AssessmentApi.Web.Model.StudentClassroom{
          id: UUID.uuid4(),
          classroom_id: args.student.classroom_id,
          student_id: args.student.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.student.classroom_id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec update_classroom(
          %{classroom: UpdateClassroomInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_classroom(args, _info) do
    classroom = AssessmentApi.Web.Model.Classroom.id(args.classroom.id, query_type: :mutation)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Classroom.Multi.update(
        classroom,
        %{
          id: args.classroom.id,
          name: args.classroom.name
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.classroom.id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec give_test_to_classroom(
          %{classroom_test: GiveTestToClassroomInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def give_test_to_classroom(args, _info) do
    classroom_test =
      [args.classroom_test.test_id]
      |> AssessmentApi.Web.Model.ClassroomTest.test_ids(query_type: :mutation)
      |> Map.values()
      |> Enum.filter(fn classroom_test ->
        classroom_test.classroom_id == args.classroom_test.classroom_id
      end)
      |> List.first()

    classroom_test
    |> case do
      nil ->
        multi =
          Ecto.Multi.new()
          |> AssessmentApi.Web.Model.ClassroomTest.Multi.insert(
            %AssessmentApi.Web.Model.ClassroomTest{
              id: UUID.uuid4(),
              test_id: args.classroom_test.test_id,
              classroom_id: args.classroom_test.classroom_id
            }
          )

        AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
        |> case do
          {:ok, _} -> {:ok, args.classroom_test.classroom_id}
          {:error, changeset} -> {:error, changeset}
        end

      _ ->
        {:ok, args.classroom_test.classroom_id}
    end
  end
end
