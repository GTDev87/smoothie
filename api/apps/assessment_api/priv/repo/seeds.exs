# Script for populating the database. You can run it as:
#
#     mix run priv/repo/seeds.exs
#
# Inside the script, you can read and write to any of your
# repositories directly:
#
#     AssessmentApi.Web.Repo.insert!(%MyApp.SomeModel{})
#
# We recommend using the bang functions (`insert!`, `update!`
# and so on) as they will fail if something goes wrong.

alias AssessmentApi.Web.Repo

num_grades =
  AssessmentApi.Web.Model.Grade.all([])
  |> Map.values()
  |> length()

no_grades = num_grades == 0

case no_grades do
  true ->
    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Pre K"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Kindergarten"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "First Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Second Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Third Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Fourth Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Fifth Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Sixth Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Seventh Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Eighth Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Nineth Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Tenth Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Eleventh Grade"
    # })

    # Repo.insert!(%AssessmentApi.Web.Model.Grade{
    #   id: UUID.uuid4(),
    #   name: "Twelveth Grade"
    # })
    Repo.insert!(%AssessmentApi.Web.Model.Grade{
      id: UUID.uuid4(),
      name: "Group 1"
    })
    Repo.insert!(%AssessmentApi.Web.Model.Grade{
      id: UUID.uuid4(),
      name: "Group 2"
    })
    Repo.insert!(%AssessmentApi.Web.Model.Grade{
      id: UUID.uuid4(),
      name: "Group 3"
    })
    Repo.insert!(%AssessmentApi.Web.Model.Grade{
      id: UUID.uuid4(),
      name: "Group 4"
    })

  false ->
    nil
end
