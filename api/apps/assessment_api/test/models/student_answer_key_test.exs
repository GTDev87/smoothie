defmodule AssessmentApi.StudentAnswerKeyTest do
  use AssessmentApi.ModelCase

  alias AssessmentApi.StudentAnswerKey

  @valid_attrs %{correct: true, id: "7488a646-e31f-11e4-aace-600308960662"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = StudentAnswerKey.changeset(%StudentAnswerKey{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = StudentAnswerKey.changeset(%StudentAnswerKey{}, @invalid_attrs)
    refute changeset.valid?
  end
end
