defmodule AssessmentApi.Repo.Migrations.CreateQuestion do
  use Ecto.Migration

  def change do
    create table(:questions, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:text, :text)
      add(:solution, :text)
      add(:auto_score, :boolean, default: false, null: false)
      add(:forced_response, :boolean, default: false, null: false)
      add(:order, :integer)

      add(:question_type, :integer, default: 0, null: false)

      add(:test_id, references(:tests, on_delete: :nothing, type: :binary_id), null: false)
      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:questions, [:test_id]))
    create(index(:questions, [:owner_id]))
  end
end
