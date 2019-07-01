defmodule AssessmentApi.Repo.Migrations.CreateClassroomTest do
  use Ecto.Migration

  def change do
    create table(:classroom_tests, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :classroom_id, references(:classrooms, on_delete: :nothing, type: :binary_id), null: false
      add :test_id, references(:tests, on_delete: :nothing, type: :binary_id), null: false

      timestamps()
    end

    create index(:classroom_tests, [:classroom_id])
    create index(:classroom_tests, [:test_id])
  end
end
