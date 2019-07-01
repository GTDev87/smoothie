defmodule AssessmentApi.Web.Schema.Domain.AnswerKeyAnalytics.Helper do
  alias AssessmentApi.Web

  # question_answer_key_id <> classroom_test_id
  def get_question_answer_key_id_and_classroom_test_id(id) do
    [question_answer_key_id, classroom_test_id] = String.split(id, " ")
    {question_answer_key_id, classroom_test_id}
  end

  def get_scores_for_question_with_student_id(loader, id, callback) do
    {question_answer_key_id, classroom_test_id} = get_question_answer_key_id_and_classroom_test_id(id)

    Web.Lib.BatchUtils.batch_through_field(loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
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
                student_answer_keys
                |> Enum.filter(fn sak -> sak.original_id === question_answer_key_id end)
                |> Enum.group_by(fn sak -> Map.get(question_to_student_map, sak.student_question_id) end)

              scores_map =
                Enum.map(student_answer_key_student_map, fn {student, student_answer_key} ->
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

  def get_student_answer_keys(loader, id, callback) do
    {question_answer_key_id, classroom_test_id} = get_question_answer_key_id_and_classroom_test_id(id)

    Web.Lib.BatchUtils.batch_through_field(loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->
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

              student_answer_keys =
                student_answer_keys
                |> Enum.filter(fn sak -> sak.original_id === question_answer_key_id end)

              callback.(loader, student_answer_keys)
            end)
          end)
        end)
      end)
    end)
  end

  def get_scores_for_question(loader, id, callback) do
    get_scores_for_question_with_student_id(loader, id, fn (loader, questions_with_student_ids) ->
      scores = questions_with_student_ids |> Enum.map(fn {_, score} -> score end)

      callback.(loader, scores)
    end)
  end
end
