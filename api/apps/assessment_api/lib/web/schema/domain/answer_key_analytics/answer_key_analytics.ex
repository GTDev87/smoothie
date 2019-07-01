defmodule AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  require Logger

  object :histogram_value do
    field :text, non_null(:string)
    field :correct, non_null(:boolean)
    field :freq, non_null(:integer)
  end

  # question_answer_key_id <> classroom_test_id
  object :answer_key_analytics do
    field :id, non_null(:id) do
      resolve(fn id, _, info -> {:ok, id} end)
    end

    field :answer_key, non_null(:question_answer_key) do
      resolve(fn id, _, info ->
        {question_answer_key_id, student_classroom_id} = AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)
        {:ok, question_answer_key_id}
      end)
    end

    field :answer_key_id, non_null(:id) do
      resolve(fn id, _, info ->
        {question_answer_key_id, student_classroom_id} = AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)
        {:ok, question_answer_key_id}
      end)
    end

    field :question_text, non_null(:string) do
      resolve(fn id, _, info ->
        {question_answer_key_id, student_classroom_id} = AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, question_answer_key_id, Web.Model.QuestionAnswerKey, :id, fn (loader, question_answer_keys) ->
          question_ids = question_answer_keys |> Enum.map(fn q -> q.question_id end)

          Web.Lib.BatchUtils.batch_through_field(loader, question_ids, Web.Model.Question, :id, fn (loader, questions) ->
            question = questions |> List.first

            {:ok, question.text}
          end)
        end)
      end)
    end

    field :objective_text, non_null(:string) do
      # name
      resolve(fn id, _, info ->
        {question_answer_key_id, student_classroom_id} = AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, question_answer_key_id, Web.Model.QuestionAnswerKey, :id, fn (loader, question_answer_keys) ->
          objective_ids = question_answer_keys |> Enum.map(fn q -> q.objective_id end)

          Web.Lib.BatchUtils.batch_through_field(loader, objective_ids, Web.Model.Objective, :id, fn (loader, objectives) ->
            objective = objectives |> List.first

            {:ok, (if (objective), do: objective.text, else: "None")}
          end)
        end)
      end)
    end

    field :discrimination, non_null(:float) do
      # discrimination
      resolve(fn id, _, info ->
        {question_answer_key_id, classroom_test_id} = Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_scores_with_student_ids(loader, classroom_test_id, fn (loader, student_and_scores) ->
              Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_scores_for_question_with_student_id(loader, id, fn (loader, answer_keys_with_student_id) ->
                # discrimination = answer_key_scores |> Statistics.mean

                masters =
                  student_and_scores
                  |> Enum.map(fn {student_id, score} -> {student_id, score / (if num_questions > 0, do: num_questions, else: 1)} end)
                  |> Enum.map(fn {s, percent} -> {s, (percent >= AssessmentApi.Web.Lib.Stats.mastery_value)} end)
                  |> Map.new

                if_master =
                  (answer_keys_with_student_id
                  |> Enum.filter(fn {s, answer} -> Map.get(masters, s) end)
                  |> Enum.map(fn {_, val} -> val end)
                  |> Statistics.mean) || 0

                if_developing =
                  (answer_keys_with_student_id
                  |> Enum.filter(fn {s, answer} -> !Map.get(masters, s) end)
                  |> Enum.map(fn {_, val} -> val end)
                  |> Statistics.mean) || 0

                {:ok, if_master - if_developing}
              end)
            end)
          end)
        end)
      end)
    end

    field :proportion, non_null(:float) do
      # proportion
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_scores_for_question(info.context.loader, id, fn (loader, answer_key_scores) ->
          proportion = answer_key_scores |> Statistics.mean
          {:ok, proportion}
        end)
      end)
    end

    field :masters, non_null(list_of(non_null(:student))) do
      # masters
      resolve(fn id, _, info ->
        {question_answer_key_id, classroom_test_id} = Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_scores_with_student_ids(loader, classroom_test_id, fn (loader, student_and_scores) ->
              Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_scores_for_question_with_student_id(loader, id, fn (loader, answer_keys_with_student_id) ->
                # discrimination = answer_key_scores |> Statistics.mean

                masters =
                  answer_keys_with_student_id
                  |> Enum.filter(fn {s, val} -> val == 1 end)
                  |> Enum.map(fn {s, _} -> s end)

                {:ok, masters}
              end)
            end)
          end)
        end)
      end)
    end

    field :masters_ids, non_null(list_of(non_null(:id))) do
      # masters
      resolve(fn id, _, info ->
        {question_answer_key_id, classroom_test_id} = Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_scores_with_student_ids(loader, classroom_test_id, fn (loader, student_and_scores) ->
              Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_scores_for_question_with_student_id(loader, id, fn (loader, answer_keys_with_student_id) ->
                # discrimination = answer_key_scores |> Statistics.mean

                masters =
                  answer_keys_with_student_id
                  |> Enum.filter(fn {s, val} -> val == 1 end)
                  |> Enum.map(fn {s, _} -> s end)

                {:ok, masters}
              end)
            end)
          end)
        end)
      end)
    end

    field :developing, non_null(list_of(non_null(:student))) do
      # developing
      resolve(fn id, _, info ->
        {question_answer_key_id, classroom_test_id} = Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_scores_with_student_ids(loader, classroom_test_id, fn (loader, student_and_scores) ->
              Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_scores_for_question_with_student_id(loader, id, fn (loader, answer_keys_with_student_id) ->
                # discrimination = answer_key_scores |> Statistics.mean

                developing =
                  answer_keys_with_student_id
                  |> Enum.filter(fn {_, val} -> val == 0 end)
                  |> Enum.map(fn {s, _} -> s end)

                {:ok, developing}
              end)
            end)
          end)
        end)
      end)
    end

    field :developing_ids, non_null(list_of(non_null(:id))) do
      # developing
      resolve(fn id, _, info ->
        {question_answer_key_id, classroom_test_id} = Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Lib.BatchUtils.batch_through_field(info.context.loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_scores_with_student_ids(loader, classroom_test_id, fn (loader, student_and_scores) ->
              Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_scores_for_question_with_student_id(loader, id, fn (loader, answer_keys_with_student_id) ->
                # discrimination = answer_key_scores |> Statistics.mean

                developing =
                  answer_keys_with_student_id
                  |> Enum.filter(fn {_, val} -> val == 0 end)
                  |> Enum.map(fn {s, _} -> s end)

                {:ok, developing}
              end)
            end)
          end)
        end)
      end)
    end

    field :student_question_answers, non_null(list_of(non_null(:student_answer_key))) do
      # developing
      resolve(fn id, _, info ->
        Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_student_answer_keys(info.context.loader, id, fn (loader, saks) ->
          # Logger.debug "saks = #{inspect saks}"
          saks_ids = saks |> Enum.map(fn sak -> sak.id end)
          {:ok, saks_ids}
        end)
      end)
    end

    field :student_question_answer_ids, non_null(list_of(non_null(:id))) do
      # developing
      resolve(fn id, _, info ->
        Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_student_answer_keys(info.context.loader, id, fn (loader, saks) ->
          # Logger.debug "saks = #{inspect saks}"
          saks_ids = saks |> Enum.map(fn sak -> sak.id end)
          {:ok, saks_ids}
        end)
      end)
    end

    field :histogram, non_null(list_of(non_null(:histogram_value))) do
      # developing
      resolve(fn id, _, info ->
        {question_answer_key_id, classroom_test_id} = Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_question_answer_key_id_and_classroom_test_id(id)

        Web.Schema.Domain.TestAnalytics.Helper.get_num_students(info.context.loader, classroom_test_id, fn (loader, num_students) ->
          Web.Schema.Domain.AnswerKeyAnalytics.Helper.get_student_answer_keys(loader, id, fn (loader, saks) ->
            student_question_ids = saks |> Enum.map(fn sak -> sak.student_question_id end)
            Web.Lib.BatchUtils.batch_through_field(loader, student_question_ids, Web.Model.StudentQuestion, :id, fn (loader, student_questions) ->
              student_question_map =
                student_questions
                |> Enum.map(fn sq -> {sq.id, sq} end)
                |> Map.new

              histogram =
                saks
                |> Enum.group_by(fn sak ->
                    {Map.get(student_question_map, sak.student_question_id).answer, sak.correct}
                  end)
                |> Enum.map(fn {key, saks} -> {key, length(saks)} end)
                |> Enum.map(fn {{text, correct}, freq} ->
                    %{
                      text: text,
                      correct: correct,
                      freq: freq
                    }
                  end)

              extra_students = num_students - length(saks)

              histogram =
                if (extra_students > 0) do
                  histogram ++ [
                    %{
                      text: "(unanswered)",
                      correct: false,
                      freq: extra_students
                    }
                  ]

                else
                  histogram
                end
              {:ok, histogram}
            end)
          end)
        end)
      end)
    end
  end
end
