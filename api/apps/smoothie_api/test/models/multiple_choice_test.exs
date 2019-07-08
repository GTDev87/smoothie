defmodule SmoothieApi.MultipleChoiceTest do
  use SmoothieApi.ModelCase

  alias SmoothieApi.MultipleChoice

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = MultipleChoice.changeset(%MultipleChoice{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = MultipleChoice.changeset(%MultipleChoice{}, @invalid_attrs)
    refute changeset.valid?
  end
end
