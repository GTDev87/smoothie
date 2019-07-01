defmodule AssessmentApi.Repo.Migrations.CreateStudentClassroom do
  use Ecto.Migration

  def change do
    create table(:student_classrooms, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)

      add(
        :classroom_id,
        references(:classrooms, on_delete: :nothing, type: :binary_id),
        null: false
      )

      add(:student_id, references(:students, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:student_classrooms, [:classroom_id]))
    create(index(:student_classrooms, [:student_id]))
  end
end
