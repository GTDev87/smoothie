defmodule AssessmentApi.QuestionTest do
  use AssessmentApi.ModelCase

  alias AssessmentApi.Question

  @valid_attrs %{auto_score: true, forced_response: true, id: "7488a646-e31f-11e4-aace-600308960662", order: 42, solution: "some solution", text: "some text"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = Question.changeset(%Question{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = Question.changeset(%Question{}, @invalid_attrs)
    refute changeset.valid?
  end
end
