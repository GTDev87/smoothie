defmodule AssessmentApi.Repo.Migrations.CreateQuestionAnswerKey do
  use Ecto.Migration

  def change do
    create table(:question_answer_keys, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:score, :float)

      add(
        :objective_id,
        references(:objectives, on_delete: :nothing, type: :binary_id),
        null: true
      )

      add(
        :question_id,
        references(:questions, on_delete: :nothing, type: :binary_id),
        null: false
      )

      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:question_answer_keys, [:objective_id]))
    create(index(:question_answer_keys, [:question_id]))
    create(index(:question_answer_keys, [:owner_id]))
  end
end
