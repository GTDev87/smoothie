defmodule AssessmentApi.Web.Schema.Domain.TestAnalytics.Helper do
  alias AssessmentApi.Web

  #id classroom_test id

  def get_scores_with_student_ids(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
      classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal
      Web.Lib.BatchUtils.batch_through_field(loader, classroom_ids, Web.Model.StudentClassroom, :classroom_id, fn (loader, student_classrooms) ->
        student_ids = student_classrooms |> Enum.map(fn sc -> sc.student_id end)
        test_ids = classroom_tests |> Enum.map(fn ct -> ct.test_id end) #filter by student... this is not optimal

        Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.StudentTest, :original_id, fn (loader, student_tests) ->
          student_test_ids =
            student_tests
            |> Enum.filter(fn st -> Enum.member?(student_ids, st.student_id) end)
            |> Enum.map(fn q -> q.id end)

          specific_student_ids =
            student_tests
            |> Enum.map(fn st -> st.student_id end)
            |> Enum.filter(fn student_id -> Enum.member?(student_ids, student_id) end)
            |> Enum.uniq


          student_test_student_map =
            student_tests
            |> Enum.map(fn st -> {st.id, st.student_id} end)
            |> Map.new

          Web.Lib.BatchUtils.batch_through_field(loader, student_test_ids, Web.Model.StudentQuestion, :student_test_id, fn (loader, student_questions) ->
            student_question_ids = student_questions |> Enum.map(fn q -> q.id end)

            question_to_student_map =
              student_questions
              |> Enum.map(fn sq -> {sq.id, Map.get(student_test_student_map, sq.student_test_id)} end)
              |> Map.new

            Web.Lib.BatchUtils.batch_through_field(loader, student_question_ids, Web.Model.StudentAnswerKey, :student_question_id, fn (loader, student_answer_keys) ->
              # note this requires a sak to exist which means that it must be selected on the frontend.
              # if not selected then excluded from calculations

              student_answer_key_student_map =
                Enum.group_by(student_answer_keys, fn sak -> Map.get(question_to_student_map, sak.student_question_id) end)

              scores_map =
                student_answer_key_student_map
                |> Enum.map(fn {student, student_answer_key} ->
                  # Logger.debug "student = #{inspect student}"
                  # Logger.debug "student_answer_key = #{inspect student_answer_key}"
                  score =
                    student_answer_key
                    |> Enum.map(fn sak -> (if sak.correct, do: 1, else: 0) end)
                    |> Enum.sum
                  {student, score}
                end)
                |> Map.new

              scores_with_zeroed_tests =
                specific_student_ids
                |> Enum.map(fn s -> {s, Map.get(scores_map, s, 0)} end)

              callback.(loader, scores_with_zeroed_tests)
            end)
          end)
        end)
      end)
    end)
  end

  def get_scores(loader, id, callback) do
    get_scores_with_student_ids(loader, id, fn (loader, scores_with_ids) ->
      scores = scores_with_ids |> Enum.map(fn {_, score} -> score end)

      callback.(loader, scores)
    end)
  end

  def get_mean(loader, id, callback) do
    get_scores(loader, id, fn (loader, scores) ->
      mean = (scores |> Statistics.mean) || 0
      callback.(loader, mean)
    end)
  end

  def get_started_student_tests(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
      classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal

      Web.Lib.BatchUtils.batch_through_field(loader, classroom_ids, Web.Model.StudentClassroom, :classroom_id, fn (loader, student_classrooms) ->
        student_ids = student_classrooms |> Enum.map(fn sc -> sc.student_id end)
        test_ids = classroom_tests |> Enum.map(fn ct -> ct.test_id end) #filter by student... this is not optimal

        Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.StudentTest, :original_id, fn (loader, student_tests) ->
          student_test_ids =
            student_tests
            |> Enum.filter(fn st -> Enum.member?(student_ids, st.student_id) end)
            |> Enum.uniq(fn st -> st.student_id end) # removes student duplicates

          callback.(loader, student_test_ids)
        end)
      end)
    end)
  end

  def get_started_student_tests(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
      classroom_ids = classroom_tests |> Enum.map(fn ct -> ct.classroom_id end) #filter by student... this is not optimal

      Web.Lib.BatchUtils.batch_through_field(loader, classroom_ids, Web.Model.StudentClassroom, :classroom_id, fn (loader, student_classrooms) ->
        student_ids = student_classrooms |> Enum.map(fn sc -> sc.student_id end)
        test_ids = classroom_tests |> Enum.map(fn ct -> ct.test_id end) #filter by student... this is not optimal

        Web.Lib.BatchUtils.batch_through_field(loader, test_ids, Web.Model.StudentTest, :original_id, fn (loader, student_tests) ->
          student_test_ids =
            student_tests
            |> Enum.filter(fn st -> Enum.member?(student_ids, st.student_id) end)
            |> Enum.uniq(fn st -> st.student_id end) # removes student duplicates

          callback.(loader, student_test_ids)
        end)
      end)
    end)
  end

  def get_prop_avg(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
      test_id =
        classroom_tests
        |> Enum.map(fn ct -> ct.test_id end)
        |> List.first
      Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
        Web.Schema.Domain.TestAnalytics.Helper.get_scores(loader, id, fn (loader, scores) ->
          score_sum = scores |> Enum.sum


          callback.(loader, (if length(scores) > 0, do: score_sum / (length(scores) * num_questions), else: 0))
        end)
      end)
    end)
  end

  def get_num_questions_from_classroom_test(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
      test_id =
        classroom_tests
        |> Enum.map(fn ct -> ct.test_id end)
        |> List.first
      Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
        callback.(loader, num_questions)
      end)
    end)
  end

  def get_var_prop(loader, id, callback) do
    get_started_student_tests(loader, id, fn (loader, student_tests) ->
      student_test_ids = student_tests |> Enum.map(fn st -> st.id end)
      num_students = length(student_test_ids)
      Web.Lib.BatchUtils.batch_map(loader, student_test_ids, &Web.Schema.Domain.StudentTestAnalytics.Helper.get_dev_sq/3, fn (loader, sq_sums) ->
        dev_sq_sum = sq_sums |> Enum.sum
        var_prop = dev_sq_sum / (if num_students > 0, do: num_students, else: 1)
        callback.(loader, var_prop)
      end)
    end)
  end

  def get_num_students(loader, id, callback) do
    Web.Schema.Domain.TestAnalytics.Helper.get_started_student_tests(loader, id, fn (loader, student_tests) ->
      callback.(loader, length(student_tests))
    end)
  end
end
