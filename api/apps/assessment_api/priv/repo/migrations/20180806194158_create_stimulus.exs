defmodule AssessmentApi.Repo.Migrations.CreateStimulus do
  use Ecto.Migration

  def change do
    create table(:stimuli, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:text, :text)

      add(
        :question_id,
        references(:questions, on_delete: :nothing, type: :binary_id),
        null: false
      )

      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:stimuli, [:question_id]))
    create(index(:stimuli, [:owner_id]))
  end
end
