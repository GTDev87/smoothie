defmodule AssessmentApi.Repo.Migrations.CreateMultipleChoice do
  use Ecto.Migration

  def change do
    create table(:multiple_choices, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)

      add(
        :question_id,
        references(:questions, on_delete: :nothing, type: :binary_id),
        null: false
      )

      timestamps()
    end

    create(index(:multiple_choices, [:question_id]))
  end
end
