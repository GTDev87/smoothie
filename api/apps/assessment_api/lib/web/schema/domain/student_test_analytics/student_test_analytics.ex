defmodule AssessmentApi.Web.Schema.Domain.StudentTestAnalytics do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web
  require Logger

  # same ids as student_tests
  object :student_test_analytics do
    field :id, non_null(:id) do
      resolve(fn id, _, info -> {:ok, id} end)
    end

    field :student_test, non_null(:student_test) do
      resolve(fn id, _, info -> {:ok, id} end)
    end

    field :student_test_id, non_null(:id) do
      resolve(fn id, _, info -> {:ok, id} end)
    end

    # field :deviation, non_null(:float) do
    #   resolve(fn id, _, info ->
    #     Web.Schema.Domain.StudentTestAnalytics.Helper.get_deviation(info.context.loader, id, fn (loader, deviation) ->
    #       require Logger
    #       Logger.debug "deviation = #{inspect deviation}"
    #       {:ok, deviation}
    #     end)
    #   end)
    # end

    # field :deviation_sq, non_null(:float) do
    #   resolve(fn id, _, info ->
    #     Web.Schema.Domain.StudentTestAnalytics.Helper.get_deviation_sq(info.context.loader, id, fn (loader, deviation_sq) ->
    #       require Logger
    #       Logger.debug "deviation_sq = #{inspect deviation_sq}"
    #       {:ok, deviation_sq}
    #     end)
    #   end)
    # end


    field :score, non_null(:float) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.score(info.context.loader, id, fn (loader, score) ->
          {:ok, score}
        end)
      end)
    end


    field :percentage, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.StudentTest, :id, fn (loader, student_tests) ->
          test_id = student_tests |> Enum.map(fn st -> st.original_id end) |> List.first
          Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
            Web.Schema.Domain.StudentTestAnalytics.Helper.score(loader, id, fn (loader, score) ->
              {:ok, (score / (if (num_questions > 0), do: num_questions, else: 1)) }
            end)
          end)
        end)
      end)
    end

    # field :mastery, non_null(:boolean) do
    #   resolve(fn id, _, info ->
    #     AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.score(info.context.loader, id, fn (loader, score) ->
    #       {:ok, score}
    #     end)
    #   end)
    # end

    # field :developing, non_null(:boolean) do
    #   resolve(fn id, _, info ->
    #     AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.score(info.context.loader, id, fn (loader, score) ->
    #       {:ok, score}
    #     end)
    #   end)
    # end

    field :name, non_null(:string) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.StudentTest, :id, fn (loader, student_tests) ->
          student_ids = student_tests |> Enum.map(fn q -> q.student_id end)
          Web.Lib.BatchUtils.batch_through_field(loader, student_ids, Web.Model.Student, :id, fn (loader, students) ->
            student = List.first(students)
            {:ok, student.first_name}
          end)
        end)
      end)
    end

    field :prop, non_null(:float) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.get_prop(info.context.loader, id, fn (loader, prop) ->
          {:ok, prop}
        end)
      end)
    end

    field :dev, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.StudentTestAnalytics.Helper.get_dev(info.context.loader, id, fn (loader, dev) ->
          {:ok, dev}
        end)
      end)
    end

    field :dev_sq, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.StudentTestAnalytics.Helper.get_dev_sq(info.context.loader, id, fn (loader, dev_sq) ->
          {:ok, dev_sq}
        end)
      end)
    end

    field :z_score, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.StudentTestAnalytics.Helper.test_to_classroom_test(info.context.loader, id, fn (loader, classroom_test) ->
          require Logger

          classroom_test_id = classroom_test.id
          Web.Schema.Domain.TestAnalytics.Helper.get_scores(loader, classroom_test_id, fn (loader, scores) ->
            stdev = scores |> Statistics.stdev
            betterstdev = (if (scores |> Statistics.stdev) == 0.0, do: 1, else: stdev)
            mean = (scores |> Statistics.mean) || 0

            Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.StudentTest, :id, fn (loader, student_tests) ->
              student_test_ids = student_tests |> Enum.map(fn q -> q.id end)

              Web.Lib.BatchUtils.batch_through_field(loader, student_test_ids, Web.Model.StudentQuestion, :student_test_id, fn (loader, student_questions) ->
                student_question_ids = student_questions |> Enum.map(fn q -> q.id end)
                question_to_test_map =
                  student_questions
                  |> Enum.map(fn sq -> {sq.id, sq.student_test_id} end)
                  |> Map.new

                Web.Lib.BatchUtils.batch_through_field(loader, student_question_ids, Web.Model.StudentAnswerKey, :student_question_id, fn (loader, student_answer_keys) ->
                  grouped_students_by_test =
                    student_answer_keys
                    |> Enum.group_by(fn sak -> Map.get(question_to_test_map, sak.student_question_id) end)

                  correct_or_not = (fn sak -> (if sak.correct, do: 1, else: 0) end)

                  my_score =
                    grouped_students_by_test
                    |> Map.get(id, [])
                    |> Enum.map(correct_or_not)
                    |> Enum.sum

                  {:ok, (my_score - mean) / betterstdev}
                end)
              end)
            end)
          end)
        end)
      end)
    end

    field :student_answer_keys, non_null(list_of(non_null(:student_answer_key))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.get_student_answer_keys(info.context.loader, id, fn (loader, student_answer_keys) ->
          {:ok, student_answer_keys |> Enum.map(fn sak -> sak.id end)}
        end)
      end)
    end

    field :student_answer_key_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.get_student_answer_keys(info.context.loader, id, fn (loader, student_answer_keys) ->
          {:ok, student_answer_keys |> Enum.map(fn sak -> sak.id end)}
        end)
      end)
    end

    # field :student_answer_key_ids, non_null(list_of(non_null(:id))) do
    #   resolve(fn id, _, info ->
    #     require Logger
    #     Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.StudentTest, :id, fn (loader, student_tests) ->
    #       Logger.debug "student_answer_key_ids test_ids"
    #       test_ids = student_tests |> Enum.map(fn q -> q.original_id end)
    #       Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.Question, :test_id, fn (loader, questions) ->
    #         Logger.debug "student_answer_key_ids question_ids"
    #         question_ids = questions |> Enum.map(fn q -> q.id end)
    #         Web.Lib.BatchUtils.batch_through_field(loader, question_ids, Web.Model.QuestionAnswerKey, :question_id, fn (_loader, question_answer_keys) ->
    #           Logger.debug "student_answer_key_ids question_answer_key_ids"
    #           question_answer_key_ids = question_answer_keys |> Enum.map(fn q -> q.id end)

    #           ids =
    #             question_answer_key_ids
    #             |> Enum.map(fn qak_id -> qak_id <> " " <> id end)

    #           {:ok, ids}
    #         end)
    #       end)
    #     end)
    #   end)
    # end
  end
end
