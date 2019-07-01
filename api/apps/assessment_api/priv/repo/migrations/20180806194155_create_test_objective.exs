defmodule AssessmentApi.Repo.Migrations.CreateTestObjective do
  use Ecto.Migration

  def change do
    create table(:test_objectives, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :test_id, references(:tests, on_delete: :nothing, type: :binary_id), null: false
      add :objective_id, references(:objectives, on_delete: :nothing, type: :binary_id), null: false

      timestamps()
    end

    create index(:test_objectives, [:test_id])
    create index(:test_objectives, [:objective_id])
  end
end
