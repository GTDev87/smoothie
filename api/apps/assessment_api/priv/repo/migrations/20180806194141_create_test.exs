defmodule AssessmentApi.Repo.Migrations.CreateTest do
  use Ecto.Migration

  def change do
    create table(:tests, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:name, :string)
      add(:description, :text)
      add(:teacher_id, references(:teachers, on_delete: :nothing, type: :binary_id), null: false)
      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:tests, [:teacher_id]))
    create(index(:tests, [:owner_id]))
  end
end
