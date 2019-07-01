defmodule AssessmentApi.TestTest do
  use AssessmentApi.ModelCase

  alias AssessmentApi.Test

  @valid_attrs %{description: "some description", id: "7488a646-e31f-11e4-aace-600308960662", name: "some name"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = Test.changeset(%Test{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = Test.changeset(%Test{}, @invalid_attrs)
    refute changeset.valid?
  end
end
