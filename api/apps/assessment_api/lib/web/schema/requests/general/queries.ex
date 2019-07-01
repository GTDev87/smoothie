defmodule AssessmentApi.Web.Schema.General.Queries do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input IdsInput do
    field(
      :ids,
      Absinthe.Schema.Notation.list_of(Absinthe.Schema.Notation.non_null(:id))
    )
  end

  def_absinthe_input RequiredIdsInput do
    field(
      :ids,
      Absinthe.Schema.Notation.non_null(
        Absinthe.Schema.Notation.list_of(Absinthe.Schema.Notation.non_null(:id))
      )
    )
  end

  object :general_query do
    # does not need auth

    field :grades, type: non_null(list_of(non_null(:grade))) do
      arg(:filter, :ids_input)

      resolve(&AssessmentApi.Web.Schema.General.Queries.get_grades/2)
    end

    field :students, type: non_null(list_of(non_null(:student))) do
      arg(:filter, non_null(:required_ids_input))

      resolve(&AssessmentApi.Web.Schema.General.Queries.get_students/2)
    end

    field :tests, type: non_null(list_of(non_null(:test))) do
      arg(:filter, non_null(:required_ids_input))

      resolve(&AssessmentApi.Web.Schema.General.Queries.get_tests/2)
    end

    field :classrooms, type: non_null(list_of(non_null(:classroom))) do
      arg(:filter, non_null(:required_ids_input))

      resolve(&AssessmentApi.Web.Schema.General.Queries.get_classrooms/2)
    end
  end

  def get_grades(_args, _info) do
    {:ok,
     AssessmentApi.Web.Model.Grade.all([])
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end

  def get_students(args, _info) do
    {:ok,
     AssessmentApi.Web.Model.Student.ids(args.filter.ids, query_type: :query)
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end

  def get_tests(args, _info) do
    {:ok,
     AssessmentApi.Web.Model.Test.ids(args.filter.ids, query_type: :query)
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end

  def get_classrooms(args, _info) do
    {:ok,
     AssessmentApi.Web.Model.Classroom.ids(args.filter.ids, query_type: :query)
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end
end
