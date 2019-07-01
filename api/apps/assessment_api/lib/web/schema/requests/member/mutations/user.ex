defmodule AssessmentApi.Web.Schema.Member.Mutations.User do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input TeacherInput do
    field(:id, non_null(:id))
    # field(:test_id, non_null(:id))
  end

  object :user_member_mutations do
    field :create_teacher, type: :user do
      arg(:teacher, :teacher_input)

      resolve(&create_teacher/2)
    end
  end

  @spec create_teacher(
          %{teacher: TeacherInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def create_teacher(args, info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Teacher.Multi.insert(%AssessmentApi.Web.Model.Teacher{
        id: args.teacher.id,
        user_id: info.context.current_user.id
      })

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, info.context.current_user.id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
