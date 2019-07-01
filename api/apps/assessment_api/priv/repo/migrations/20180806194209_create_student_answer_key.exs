defmodule AssessmentApi.Repo.Migrations.CreateStudentAnswerKey do
  use Ecto.Migration

  def change do
    create table(:student_answer_keys, primary_key: false) do
      add :id, :binary_id, primary_key: true, null: false
      add :correct, :boolean, default: false, null: false
      add :original_id, references(:question_answer_keys, on_delete: :nothing, type: :binary_id), null: false
      add :student_question_id, references(:student_questions, on_delete: :nothing, type: :binary_id), null: false

      timestamps()
    end

    create index(:student_answer_keys, [:original_id])
    create index(:student_answer_keys, [:student_question_id])
  end
end
