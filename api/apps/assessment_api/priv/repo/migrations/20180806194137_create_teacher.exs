defmodule AssessmentApi.Repo.Migrations.CreateTeacher do
  use Ecto.Migration

  def change do
    create table(:teachers, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:user_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:teachers, [:user_id], unique: true))
  end
end
