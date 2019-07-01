defmodule AssessmentApi.Web.Schema.Domain.TestAnalytics do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  require Logger

  # need to query classroom test
  # ot get access to students need to go back up through classroom

  object :test_analytics do
    # same id as classroom_test
    field :id, non_null(:id) do
      resolve(Web.Lib.BatchUtils.get_field(Web.Model.ClassroomTest, :id))
    end

    field :test_id, non_null(:id) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first
          {:ok, test_id}
        end)
      end)
    end

    field :started_students, non_null(list_of(non_null(:student))) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_started_student_tests(info.context.loader, id, fn (loader, student_tests) ->
          {:ok, student_tests |> Enum.map(fn st -> st.student_id end)}
        end)
      end)
    end

    field :started_student_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_started_student_tests(info.context.loader, id, fn (loader, student_tests) ->
          {:ok, student_tests |> Enum.map(fn st -> st.student_id end)}
        end)
      end)
    end

    field :unstarted_students, non_null(list_of(non_null(:student))) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal

          Web.Lib.BatchUtils.batch_through_field(loader, classroom_ids, Web.Model.StudentClassroom, :classroom_id, fn (loader, student_classrooms) ->
            student_ids = student_classrooms |> Enum.map(fn sc -> sc.student_id end)
            test_ids = classroom_tests |> Enum.map(fn ct -> ct.test_id end) #filter by student... this is not optimal


            Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.StudentTest, :original_id, fn (loader, student_tests) ->
              student_test_ids =
                student_tests
                |> Enum.filter(fn st -> Enum.member?(student_ids, st.student_id) end)
                |> Enum.map(fn q -> q.id end)

              student_test_to_student =
                student_tests
                |> Enum.map(fn st -> {st.id, st.student_id} end)
                |> Map.new

              Web.Lib.BatchUtils.batch_through_field(loader, student_test_ids, Web.Model.StudentQuestion, :student_test_id, fn (loader, student_questions) ->
                sq_map_count =
                  student_questions
                  |> Enum.group_by(fn sq -> Map.get(student_test_to_student, sq.student_test_id) end)
                  |> Enum.map(fn {s_id, list_sqs} -> {s_id, length(list_sqs)} end)
                  |> Map.new

                test_unstarted_students =
                  student_ids
                  |> Enum.filter(fn s_id -> !Map.get(sq_map_count, s_id) end)
                {:ok, test_unstarted_students}
              end)
            end)
          end)
        end)
      end)
    end

    field :unstarted_student_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal

          Web.Lib.BatchUtils.batch_through_field(loader, classroom_ids, Web.Model.StudentClassroom, :classroom_id, fn (loader, student_classrooms) ->
            student_ids = student_classrooms |> Enum.map(fn sc -> sc.student_id end)
            test_ids = classroom_tests |> Enum.map(fn ct -> ct.test_id end) #filter by student... this is not optimal


            Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.StudentTest, :original_id, fn (loader, student_tests) ->
              student_test_ids =
                student_tests
                |> Enum.filter(fn st -> Enum.member?(student_ids, st.student_id) end)
                |> Enum.map(fn q -> q.id end)

              student_test_to_student =
                student_tests
                |> Enum.map(fn st -> {st.id, st.student_id} end)
                |> Map.new

              Web.Lib.BatchUtils.batch_through_field(loader, student_test_ids, Web.Model.StudentQuestion, :student_test_id, fn (loader, student_questions) ->
                sq_map_count =
                  student_questions
                  |> Enum.group_by(fn sq -> Map.get(student_test_to_student, sq.student_test_id) end)
                  |> Enum.map(fn {s_id, list_sqs} -> {s_id, length(list_sqs)} end)
                  |> Map.new

                test_unstarted_students =
                  student_ids
                  |> Enum.filter(fn s_id -> !Map.get(sq_map_count, s_id) end)


                {:ok, test_unstarted_students}
              end)
            end)
          end)
        end)
      end)
    end

    field :test_name, non_null(:id) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first
          Web.Lib.BatchUtils.batch_through_field(loader, test_id, Web.Model.Test, :id, fn (loader, tests) ->
            test_name = tests |> Enum.map(fn ct -> ct.name end) |> List.first
            {:ok, test_name}
          end)
        end)
      end)
    end

    field :test, non_null(:test) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first
          {:ok, test_id}
        end)
      end)
    end

    field :total_score, non_null(:integer) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          test_ids = classroom_tests |> Enum.map(fn ct -> ct.test_id end)
          Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.Question, :test_id, fn (loader, questions) ->
            question_ids = questions |> Enum.map(fn q -> q.id end)

            Web.Lib.BatchUtils.batch_through_field(loader, question_ids, Web.Model.QuestionAnswerKey, :question_id, fn (loader, question_answer_keys) ->
              score =
                question_answer_keys
                |> Enum.map(fn t ->  t.score end)
                |> Enum.sum

              {:ok, score || 0}
            end)
          end)
        end)
      end)
    end

    field :min, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_scores(info.context.loader, id, fn (loader, scores) ->
          min_score = scores |> Enum.min(fn -> 0 end)
          {:ok, min_score || 0}
        end)
      end)
    end

    field :max, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_scores(info.context.loader, id, fn (loader, scores) ->
          max_score = scores |> Enum.max(fn -> 0 end)
          {:ok, max_score || 0}
        end)
      end)
    end

    field :median, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_scores(info.context.loader, id, fn (loader, scores) ->
          median_score = scores |> Statistics.median
          {:ok, median_score || 0}
        end)
      end)
    end

    field :num_students, non_null(:integer) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_num_students(info.context.loader, id, fn (loader, num_students) ->
          {:ok, num_students}
        end)
      end)
    end

    field :mean, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_mean(info.context.loader, id, fn (loader, mean) ->
          {:ok, mean || 0}
        end)
      end)
    end

    field :standard_deviation, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_scores(info.context.loader, id, fn (loader, scores) ->
          stdev = scores |> Statistics.stdev
          num_scores = length(scores)
          sample_stdev = if (stdev == nil || num_scores < 2), do: 1, else: stdev * (:math.sqrt((num_scores / (num_scores - 1))))

          {:ok, sample_stdev || 1}
        end)
      end)
    end

    field :prop_avg, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_prop_avg(info.context.loader, id, fn (loader, prop_avg) ->
          {:ok, prop_avg}
        end)
      end)
    end

    field :var_prop, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_var_prop(info.context.loader, id, fn (loader, var_prop) ->
          {:ok, var_prop}
        end)
      end)
    end

    field :phi, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_var_prop(info.context.loader, id, fn (loader, var_prop) ->
          Web.Schema.Domain.TestAnalytics.Helper.get_prop_avg(loader, id, fn (loader, prop_avg) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_num_questions_from_classroom_test(loader, id, fn (loader, num_questions) ->
              require Logger
              mastery = AssessmentApi.Web.Lib.Stats.mastery_value
              numerator = ((prop_avg * (1 - prop_avg)) - var_prop)
              denominator = ((prop_avg - mastery) * (prop_avg - mastery)) + var_prop
              phi = 1 - (((numerator) / (denominator)) * (1 / (num_questions - 1)))

              {:ok, phi}
            end)
          end)
        end)
      end)
    end

    field :reliability, non_null(:float) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_var_prop(info.context.loader, id, fn (loader, var_prop) ->
          Web.Schema.Domain.TestAnalytics.Helper.get_prop_avg(loader, id, fn (loader, prop_avg) ->
            Web.Schema.Domain.TestAnalytics.Helper.get_num_questions_from_classroom_test(loader, id, fn (loader, num_questions) ->

              mastery = AssessmentApi.Web.Lib.Stats.mastery_value
              numerator = ((prop_avg * (1 - prop_avg)) - var_prop)
              denominator = ((prop_avg - mastery) * (prop_avg - mastery)) + var_prop
              phi = 1 - (((numerator) / (denominator)) * (1 / (num_questions - 1)))

              {:ok, phi}
            end)
          end)
        end)
      end)
    end

    field :student_test_analytics, non_null(list_of(non_null(:student_test_analytics))) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_started_student_tests(info.context.loader, id, fn (loader, student_tests) ->
          {:ok, student_tests |> Enum.map(fn st -> st.id end)}
        end)
      end)
    end

    field :student_test_analytics_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        Web.Schema.Domain.TestAnalytics.Helper.get_started_student_tests(info.context.loader, id, fn (loader, student_tests) ->
          {:ok, student_tests |> Enum.map(fn st -> st.id end)}
        end)
      end)
    end

    field :answer_key_analytics, non_null(list_of(non_null(:answer_key_analytics))) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Lib.BatchUtils.batch_through_field(loader, test_id, Web.Model.Question, :test_id, fn (loader, questions) ->
            question_ids = questions |> Enum.map(fn q -> q.id end)

            Web.Lib.BatchUtils.batch_through_field(loader, question_ids, Web.Model.QuestionAnswerKey, :question_id, fn (loader, question_answer_keys) ->
              question_answer_key_ids =
                question_answer_keys
                |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
                |> Enum.map(fn q -> q.id end)

              ids =
                question_answer_key_ids
                |> Enum.map(fn qak_id -> qak_id <> " " <> id end)

              {:ok, ids}
            end)
          end)
        end)
      end)
    end

    field :answer_key_analytics_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
          classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal
          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

          Web.Lib.BatchUtils.batch_through_field(loader, test_id, Web.Model.Question, :test_id, fn (loader, questions) ->
            question_ids = questions |> Enum.map(fn q -> q.id end)

            Web.Lib.BatchUtils.batch_through_field(loader, question_ids, Web.Model.QuestionAnswerKey, :question_id, fn (loader, question_answer_keys) ->
              question_answer_key_ids = question_answer_keys |> Enum.map(fn q -> q.id end)

              ids =
                question_answer_key_ids
                |> Enum.map(fn qak_id -> qak_id <> " " <> id end)

              {:ok, ids}
            end)
          end)
        end)
      end)
    end

    field :objective_analytics, non_null(list_of(non_null(:objective_analytics))) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->

          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first
          Web.Lib.BatchUtils.batch_through_field(loader, test_id, Web.Model.TestObjective, :test_id, fn (loader, test_objectives) ->
            objective_analytics_string = test_objectives |> Enum.map(fn t -> t.objective_id <> " " <> id end)
            {:ok, objective_analytics_string}
          end)
        end)
      end)
    end

    field :objective_analytics_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->

          test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first
          Web.Lib.BatchUtils.batch_through_field(loader, test_id, Web.Model.TestObjective, :test_id, fn (loader, test_objectives) ->

            {:ok, test_objectives |> Enum.map(fn t -> t.objective_id <> " " <> id end)}
          end)
        end)
      end)
    end
  end
end
