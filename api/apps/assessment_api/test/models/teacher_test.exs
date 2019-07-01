defmodule AssessmentApi.TeacherTest do
  use AssessmentApi.ModelCase

  alias AssessmentApi.Teacher

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662", user_id: "7488a646-e31f-11e4-aace-600308960662"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = Teacher.changeset(%Teacher{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = Teacher.changeset(%Teacher{}, @invalid_attrs)
    refute changeset.valid?
  end
end
