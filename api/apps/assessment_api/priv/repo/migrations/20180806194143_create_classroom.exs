defmodule AssessmentApi.Repo.Migrations.CreateClassroom do
  use Ecto.Migration

  def change do
    create table(:classrooms, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:name, :string)
      add(:teacher_id, references(:teachers, on_delete: :nothing, type: :binary_id), null: false)
      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:classrooms, [:teacher_id]))
    create(index(:classrooms, [:owner_id]))
  end
end
