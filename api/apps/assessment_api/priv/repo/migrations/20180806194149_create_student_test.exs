defmodule AssessmentApi.Repo.Migrations.CreateStudentTest do
  use Ecto.Migration

  def change do
    create table(:student_tests, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :original_id, references(:tests, on_delete: :nothing, type: :binary_id), null: false
      add :student_id, references(:students, on_delete: :nothing, type: :binary_id), null: false

      timestamps()
    end

    create index(:student_tests, [:original_id])
    create index(:student_tests, [:student_id])
  end
end
