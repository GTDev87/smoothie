defmodule SmoothieApi.StudentClassroomTest do
  use SmoothieApi.ModelCase

  alias SmoothieApi.StudentClassroom

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = StudentClassroom.changeset(%StudentClassroom{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = StudentClassroom.changeset(%StudentClassroom{}, @invalid_attrs)
    refute changeset.valid?
  end
end
