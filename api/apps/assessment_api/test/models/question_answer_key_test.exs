defmodule AssessmentApi.QuestionAnswerKeyTest do
  use AssessmentApi.ModelCase

  alias AssessmentApi.QuestionAnswerKey

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662", score: 120.5}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = QuestionAnswerKey.changeset(%QuestionAnswerKey{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = QuestionAnswerKey.changeset(%QuestionAnswerKey{}, @invalid_attrs)
    refute changeset.valid?
  end
end
