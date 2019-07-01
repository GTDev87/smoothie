defmodule AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper do
  alias AssessmentApi.Web

  require Logger

  # objective id <> " " <>  classroom test id

  def get_objective_id_and_classroom_test_id(id) do
    [objective_id, classroom_test_id] = String.split(id, " ")
    {objective_id, classroom_test_id}
  end

  def get_student_answer_keys_for_objective(loader, id, callback) do
    {objective_id, classroom_test_id} = get_objective_id_and_classroom_test_id(id)

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

              question_answer_key_ids =
                student_answer_keys
                |> Enum.map(fn sak -> sak.original_id end)

              Web.Lib.BatchUtils.batch_through_field(loader, question_answer_key_ids, Web.Model.QuestionAnswerKey, :id, fn (loader, question_answer_keys) ->
                # note this requires a sak to exist which means that it must be selected on the frontend.
                # if not selected then excluded from calculations
                # Logger.debug "question_answer_keys = #{inspect question_answer_keys}"

                qak_map =
                  question_answer_keys
                  |> Enum.map(fn qak -> {qak.id, qak.objective_id == objective_id} end)
                  |> Map.new

                chosen_objective_student_answer_keys =
                  student_answer_keys
                  |> Enum.filter(fn sak -> Map.get(qak_map, sak.original_id, false) end)

                callback.(loader, chosen_objective_student_answer_keys)
              end)
            end)
          end)
        end)
      end)
    end)
  end

  def get_student_answer_keys_for_objective_by_student(loader, id, callback) do
    {objective_id, classroom_test_id} = get_objective_id_and_classroom_test_id(id)
    # {question_answer_key_id, classroom_test_id} = get_question_answer_key_id_and_classroom_test_id(id)

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

              question_answer_key_ids =
                student_answer_keys
                |> Enum.map(fn sak -> sak.original_id end)

              Web.Lib.BatchUtils.batch_through_field(loader, question_answer_key_ids, Web.Model.QuestionAnswerKey, :id, fn (loader, question_answer_keys) ->
                # note this requires a sak to exist which means that it must be selected on the frontend.
                # if not selected then excluded from calculations

                qak_map =
                  question_answer_keys
                  |> Enum.map(fn qak -> {qak.id, qak.objective_id == objective_id} end)
                  |> Map.new

                chosen_objective_student_answer_keys_by_student =
                  student_answer_keys
                  |> Enum.filter(fn sak -> Map.get(qak_map, sak.original_id, false) end)
                  |> Enum.group_by(fn sak -> Map.get(question_to_student_map, sak.student_question_id) end)

                callback.(loader, chosen_objective_student_answer_keys_by_student)
              end)
            end)
          end)
        end)
      end)
    end)
  end
end
