defmodule AssessmentApi.Repo.Migrations.CreateStudent do
  use Ecto.Migration

  def change do
    create table(:students, primary_key: false) do
      add(:id, :binary_id, primary_key: true, null: false)
      add(:first_name, :string)
      add(:last_name, :string)
      add(:grade_id, references(:grades, on_delete: :nothing, type: :binary_id))
      add(:owner_id, references(:users, on_delete: :nothing, type: :binary_id), null: false)

      timestamps()
    end

    create(index(:students, [:grade_id]))
    create(index(:students, [:owner_id]))
  end
end
