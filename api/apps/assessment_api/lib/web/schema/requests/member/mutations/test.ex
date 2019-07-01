defmodule AssessmentApi.Web.Schema.Member.Mutations.Test do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  import Ecto.Query

  def_absinthe_input QuestionCreateInput do
    field(:id, non_null(:id))
    field(:test_id, non_null(:id))
  end

  def_absinthe_input TestFieldsInput do
    field(:id, non_null(:id))
    field(:name, non_null(:string))
    field(:notes, non_null(:string))
    field(:description, non_null(:string))
  end

  def_absinthe_input TestObjectiveInput do
    field(:id, non_null(:id))
    field(:test_id, non_null(:id))
    field(:objective_id, non_null(:id))
  end

  def_absinthe_input UpdateQuestionTypeInput do
    field(:id, non_null(:id))
    field(:test_id, non_null(:id))

    field(
      :question_type,
      non_null(:question_type),
      AssessmentApi.Web.Schema.Domain.Question.QuestionType.t()
    )
  end

  object :test_member_mutations do
    field :add_question, type: :test do
      arg(:question, :question_create_input)

      resolve(&add_question/2)
    end

    field :add_objective, type: :test do
      arg(:test_objective, :test_objective_input)

      resolve(&add_objective/2)
    end

    field :update_test, type: :test do
      arg(:test, :test_fields_input)

      resolve(&update_test/2)
    end

    field :update_question_type, type: :test do
      arg(:question, :update_question_type_input)

      resolve(&update_question_type/2)
    end
  end

  @spec add_question(
          %{question: QuestionCreateInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_question(args, info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Question.Multi.insert(%AssessmentApi.Web.Model.Question{
        id: args.question.id,
        test_id: args.question.test_id,
        owner_id: info.context.current_user.id
      })
      |> AssessmentApi.Web.Model.QuestionAnswerKey.Multi.insert(
        %AssessmentApi.Web.Model.QuestionAnswerKey{
          id: UUID.uuid4(),
          score: 1.0,
          objective_id: nil,
          question_id: args.question.id,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.question.test_id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec add_objective(
          %{test_objective: TestObjectiveInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_objective(args, _info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.TestObjective.Multi.insert(
        %AssessmentApi.Web.Model.TestObjective{
          id: args.test_objective.id,
          test_id: args.test_objective.test_id,
          objective_id: args.test_objective.objective_id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.test_objective.test_id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec update_test(
          %{test: TestFieldsInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_test(args, info) do
    test = AssessmentApi.Web.Model.Test.id(args.test.id, query_type: :mutation)

    notes = (if args.test.notes == "", do: nil, else: args.test.notes)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Test.Multi.update(
        test,
        %{
          id: args.test.id,
          name: args.test.name,
          notes: notes,
          description: args.test.description,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.test.id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  #### THIS ON QUESTION FEELS SUPER WRONG
  @spec update_question_type(
          %{question: UpdateQuestionTypeInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_question_type(args, _info) do
    question = AssessmentApi.Web.Model.Question.id(args.question.id, query_type: :mutation)

    multiple_choice =
      AssessmentApi.Web.Model.MultipleChoice.question_id(args.question.id, query_type: :mutation)

    multiple_choice_id = UUID.uuid4()

    multi =
      Ecto.Multi.new()
      |> (fn m ->
            case multiple_choice do
              nil ->
                m

              multiple_choice ->
                multiple_choice_id = multiple_choice.id

                m
                |> Ecto.Multi.delete_all(
                  :remove_choices,
                  from(AssessmentApi.Web.Model.Choice,
                    where: [multiple_choice_id: ^multiple_choice_id]
                  )
                )
                |> AssessmentApi.Web.Model.MultipleChoice.Multi.delete(multiple_choice)
            end
          end).()
      |> (fn m ->
            case args.question.question_type do
              type when type in [:multiple_choice, :true_false] ->
                m
                |> AssessmentApi.Web.Model.MultipleChoice.Multi.insert(
                  %AssessmentApi.Web.Model.MultipleChoice{
                    id: multiple_choice_id,
                    question_id: args.question.id
                  }
                )
              _ -> m
            end
          end).()
      |> (fn m ->
        case args.question.question_type do
          :true_false ->
            m
            |> AssessmentApi.Web.Model.Choice.Multi.insert(
                %AssessmentApi.Web.Model.Choice{
                  id: UUID.uuid4(),
                  text: "true",
                  multiple_choice_id: multiple_choice_id,
                }, suffix: "true")
            |> AssessmentApi.Web.Model.Choice.Multi.insert(
              %AssessmentApi.Web.Model.Choice{
                id: UUID.uuid4(),
                text: "false",
                multiple_choice_id: multiple_choice_id,

              }, suffix: "false")
          _ -> m
        end
      end).()
      |> AssessmentApi.Web.Model.Question.Multi.update(
        question,
        %{
          id: args.question.id,
          question_type:
            AssessmentApi.Web.Schema.Domain.Question.QuestionType.to_val(
              args.question.question_type
            )
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.question.test_id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
