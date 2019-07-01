defmodule AssessmentApi.Web.Repo.Migrations.AddNotesToQuestions do
  use Ecto.Migration

  def change do
    alter table(:tests) do
      add :notes,    :string
    end
  end
end
