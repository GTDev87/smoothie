defmodule SmoothieApi.ClassroomTestTest do
  use SmoothieApi.ModelCase

  alias SmoothieApi.ClassroomTest

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = ClassroomTest.changeset(%ClassroomTest{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = ClassroomTest.changeset(%ClassroomTest{}, @invalid_attrs)
    refute changeset.valid?
  end
end
