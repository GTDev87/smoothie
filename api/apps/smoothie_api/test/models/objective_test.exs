defmodule SmoothieApi.ObjectiveTest do
  use SmoothieApi.ModelCase

  alias SmoothieApi.Objective

  @valid_attrs %{id: "7488a646-e31f-11e4-aace-600308960662", text: "some text"}
  @invalid_attrs %{}

  test "changeset with valid attributes" do
    changeset = Objective.changeset(%Objective{}, @valid_attrs)
    assert changeset.valid?
  end

  test "changeset with invalid attributes" do
    changeset = Objective.changeset(%Objective{}, @invalid_attrs)
    refute changeset.valid?
  end
end
