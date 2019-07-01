defmodule AssessmentApi.Repo.Migrations.CreateChoice do
  use Ecto.Migration

  def change do
    create table(:choices, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :text, :string
      add :multiple_choice_id, references(:multiple_choices, on_delete: :nothing, type: :binary_id), null: false

      timestamps()
    end

    create index(:choices, [:multiple_choice_id])
  end
end
