defmodule AssessmentApi.Repo.Migrations.CreateGrade do
  use Ecto.Migration

  def change do
    create table(:grades, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :name, :string

      timestamps()
    end
  end
end
