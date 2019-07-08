defmodule SmoothieApi.Web.Schema.General.Queries do
  use Absinthe.Schema.Notation
  use SmoothieApi.Web.Lib.AbsintheInputUtils

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
  end

  def get_grades(_args, _info) do
    {:ok,
     SmoothieApi.Web.Model.Grade.all([])
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end

  def get_students(args, _info) do
    {:ok,
     SmoothieApi.Web.Model.Student.ids(args.filter.ids, query_type: :query)
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end

  def get_tests(args, _info) do
    {:ok,
     SmoothieApi.Web.Model.Test.ids(args.filter.ids, query_type: :query)
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end

  def get_classrooms(args, _info) do
    {:ok,
     SmoothieApi.Web.Model.Classroom.ids(args.filter.ids, query_type: :query)
     |> Map.values()
     |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
     |> Enum.map(fn g -> g.id end)}
  end
end
