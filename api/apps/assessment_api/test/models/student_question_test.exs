defmodule AssessmentApi.StudentQuestionTest do
  use AssessmentApi.ModelCase

  alias AssessmentApi.StudentQuestion

  @valid_attrs %{answer: "some answer", id: "7488a646-e31f-11e4-aace-600308960662"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = StudentQuestion.changeset(%StudentQuestion{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = StudentQuestion.changeset(%StudentQuestion{}, @invalid_attrs)
    refute changeset.valid?
  end
end
