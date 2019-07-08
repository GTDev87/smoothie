defmodule SmoothieApi.StudentTest do
  use SmoothieApi.ModelCase

  alias SmoothieApi.Student

  @valid_attrs %{first_name: "some first_name", id: "7488a646-e31f-11e4-aace-600308960662", last_name: "some last_name"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = Student.changeset(%Student{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = Student.changeset(%Student{}, @invalid_attrs)
    refute changeset.valid?
  end
end
