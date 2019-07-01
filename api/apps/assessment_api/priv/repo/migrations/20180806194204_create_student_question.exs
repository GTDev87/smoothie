defmodule AssessmentApi.Repo.Migrations.CreateStudentQuestion do
  use Ecto.Migration

  def change do
    create table(:student_questions, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :answer, :text
      add :original_id, references(:questions, on_delete: :nothing, type: :binary_id), null: false
      add :student_test_id, references(:student_tests, on_delete: :nothing, type: :binary_id), null: false

      timestamps()
    end

    create index(:student_questions, [:original_id])
    create index(:student_questions, [:student_test_id])
  end
end
