defmodule AssessmentApi.Repo.Migrations.CreateObjective do
  use Ecto.Migration

  def change do
    create table(:objectives, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:text, :string)
      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:objectives, [:owner_id]))
  end
end
