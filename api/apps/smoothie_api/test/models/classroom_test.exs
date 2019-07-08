defmodule SmoothieApi.ClassroomTest do
  use SmoothieApi.ModelCase

  alias SmoothieApi.Classroom

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662", name: "some name"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = Classroom.changeset(%Classroom{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = Classroom.changeset(%Classroom{}, @invalid_attrs)
    refute changeset.valid?
  end
end
