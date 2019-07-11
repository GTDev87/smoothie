# defmodule SmoothieApi.Web.Schema.Domain.User.Mutations.CreateProfile.GraphQL do
#   use Absinthe.Schema.Notation
#   use SmoothieApi.Web.Lib.AbsintheInputUtils

#   def_absinthe_input TeacherInput do
#     field(:id, non_null(:id))
#   end

#   object :create_profile do
#     field :create_teacher, type: :user do
#       arg(:teacher, :teacher_input)

#       resolve(&execute/2)
#     end
#   end

#   @spec execute(%{teacher: TeacherInput.t()}, SmoothieApi.Guardian.Context.info()) :: {:error, any()} | {:ok, String}
#   def execute(args, info) do
#     # multi =
#     #   Ecto.Multi.new()
#     #   |> SmoothieApi.Web.Model.Teacher.Multi.insert(%SmoothieApi.Web.Model.Teacher{
#     #     id: args.teacher.id,
#     #     user_id: info.context.current_user.id
#     #   })

#     # SmoothieApi.Web.ReadWriteRepo.transaction(:mutation, multi)
#     # |> case do
#     #   {:ok, _} -> {:ok, info.context.current_user.id}
#     #   {:error, changeset} -> {:error, changeset}
#     # end
#     {:ok, nil}
#   end
# end