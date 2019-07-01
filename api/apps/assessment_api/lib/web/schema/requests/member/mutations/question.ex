defmodule AssessmentApi.Web.Schema.Member.Mutations.Question do
  use Absinthe.Schema.Notation
  use AssessmentApi.Web.Lib.AbsintheInputUtils

  def_absinthe_input AddQuestionAnswerKeyInput do
    field(:id, non_null(:id))
    field(:question_id, non_null(:id))
  end

  def_absinthe_input UpdateQuestionInput do
    field(:id, non_null(:id))
    field(:text, non_null(:string))
    field(:solution, non_null(:string))
    field(:auto_score, non_null(:boolean))
    field(:forced_response, non_null(:boolean))
  end

  def_absinthe_input AddStimulusInput do
    field(:id, non_null(:id))
    field(:question_id, non_null(:id))
  end

  object :question_member_mutations do
    field :add_answer_key, type: :question do
      arg(:question_answer_key, :add_question_answer_key_input)

      resolve(&add_answer_key/2)
    end

    field :update_question, type: :question do
      arg(:question, :update_question_input)

      resolve(&update_question/2)
    end

    field :add_stimulus, type: :question do
      arg(:stimulus, :add_stimulus_input)

      resolve(&add_stimulus/2)
    end
  end

  @spec add_answer_key(
          %{question_answer_key: AddQuestionAnswerKeyInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_answer_key(args, info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.QuestionAnswerKey.Multi.insert(
        %AssessmentApi.Web.Model.QuestionAnswerKey{
          id: args.question_answer_key.id,
          score: 1.0,
          objective_id: nil,
          question_id: args.question_answer_key.question_id,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.question_answer_key.question_id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec update_question(
          %{question: UpdateQuestionInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def update_question(args, info) do
    question = AssessmentApi.Web.Model.Question.id(args.question.id, query_type: :mutation)

    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Question.Multi.update(
        question,
        %{
          id: args.question.id,
          text: args.question.text,
          solution: args.question.solution,
          auto_score: args.question.auto_score,
          forced_response: args.question.forced_response,
          owner_id: info.context.current_user.id
        }
      )

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.question.id}
      {:error, changeset} -> {:error, changeset}
    end
  end

  @spec add_stimulus(
          %{stimulus: AddStimulusInput.t()},
          AssessmentApi.Guardian.Context.info()
        ) :: {:error, any()} | {:ok, String}
  def add_stimulus(args, info) do
    multi =
      Ecto.Multi.new()
      |> AssessmentApi.Web.Model.Stimulus.Multi.insert(%AssessmentApi.Web.Model.Stimulus{
        id: args.stimulus.id,
        question_id: args.stimulus.question_id,
        owner_id: info.context.current_user.id
      })

    AssessmentApi.Web.ReadWriteRepo.transaction(:mutation, multi)
    |> case do
      {:ok, _} -> {:ok, args.stimulus.question_id}
      {:error, changeset} -> {:error, changeset}
    end
  end
end
