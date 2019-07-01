defmodule AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics do
  use Absinthe.Schema.Notation
  import Absinthe.Resolution.Helpers, only: [on_load: 2]

  alias AssessmentApi.Web

  require Logger

  # same ids as objective id <> " " <>  classroom test id
  object :objective_analytics do
    field :id, non_null(:id) do
      resolve(fn id, _, info ->
        {:ok, id}
      end)
    end

    field :objective_name, non_null(:string) do
      resolve(fn id, _, info ->
        {objective_id, classroom_test_id} = AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_objective_id_and_classroom_test_id(id)
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, objective_id, Web.Model.Objective, :id, fn (loader, objectives) ->
          text = objectives |> Enum.map(fn obj -> obj.text end) |> List.first
          {:ok, text}
        end)
      end)
    end

    field :percentage_correct, non_null(:float) do
      resolve(fn id, _, info ->
        {objective_id, classroom_test_id} = AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_objective_id_and_classroom_test_id(id)
        Web.Lib.BatchUtils.batch_through_field(info.context.loader, classroom_test_id, Web.Model.ClassroomTest, :id, fn (loader, classroom_tests) ->

          AssessmentApi.Web.Schema.Domain.TestAnalytics.Helper.get_num_students(loader, classroom_test_id, fn (loader, num_students) ->
            test_id = classroom_tests |> Enum.map(fn ct -> ct.test_id end) |> List.first

            AssessmentApi.Web.Schema.Domain.Test.Helper.get_answer_keys(loader, test_id, fn (loader, answer_keys) ->

              AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective(loader, id, fn (loader, student_answer_keys) ->
                num_student_answer_keys = length(student_answer_keys)
                correct_sak =
                  student_answer_keys
                  |> Enum.filter(fn sak -> sak.correct end)
                  |> length

                num_questions =
                  answer_keys
                  |> Enum.filter(fn ak -> ak.objective_id == objective_id end)
                  |> length

                denom = num_students * num_questions
                denom = (if (denom > 0), do: denom, else: 1)

                percentage_correct = (correct_sak / denom)

                {:ok, percentage_correct}
              end)
            end)
          end)
        end)
      end)
    end

    field :questions, non_null(list_of(non_null(:question_base))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective(info.context.loader, id, fn (loader, student_answer_keys) ->
          question_answer_key_ids = student_answer_keys |> Enum.map(fn ct -> ct.original_id end)

          Web.Lib.BatchUtils.batch_through_field(loader, question_answer_key_ids, Web.Model.QuestionAnswerKey, :id, fn (loader, question_answer_keys) ->
            question_ids =
              question_answer_keys
              |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
              |> Enum.map(fn qak -> qak.question_id end)
              |> Enum.uniq

            {:ok, question_ids}
          end)
        end)
      end)
    end

    field :question_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective(info.context.loader, id, fn (loader, student_answer_keys) ->
          question_answer_key_ids = student_answer_keys |> Enum.map(fn ct -> ct.original_id end)

          Web.Lib.BatchUtils.batch_through_field(loader, question_answer_key_ids, Web.Model.QuestionAnswerKey, :id, fn (loader, question_answer_keys) ->
            question_ids =
              question_answer_keys
              |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt))
              |> Enum.map(fn qak -> qak.question_id end)
              |> Enum.uniq

            {:ok, question_ids}
          end)
        end)
      end)
    end

    field :master_students, non_null(list_of(non_null(:student))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective_by_student(info.context.loader, id, fn (loader, sak_by_student) ->
          master_students =
            sak_by_student
            |> Enum.filter(fn {student_id, saks} ->

              if (length(saks) > 0),
                do: (((Enum.filter(saks, fn sak -> sak.correct end)) |> length) / length(saks)) > AssessmentApi.Web.Lib.Stats.mastery_value,
                else: false
            end)
            |> Enum.map(fn {s, _} -> s end)
          {:ok, master_students}
        end)
      end)
    end

    field :master_student_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective_by_student(info.context.loader, id, fn (loader, sak_by_student) ->
          master_students =
            sak_by_student
            |> Enum.filter(fn {student_id, saks} ->
              if (length(saks) > 0),
                do: (((Enum.filter(saks, fn sak -> sak.correct end)) |> length) / length(saks)) > AssessmentApi.Web.Lib.Stats.mastery_value,
                else: false
            end)
            |> Enum.map(fn {s, _} -> s end)
          {:ok, master_students}
        end)
      end)
    end

    field :developing_students, non_null(list_of(non_null(:student))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective_by_student(info.context.loader, id, fn (loader, sak_by_student) ->
          developing_students =
            sak_by_student
            |> Enum.filter(fn {student_id, saks} ->
              !(if (length(saks) > 0), do: (((Enum.filter(saks, fn sak -> sak.correct end)) |> length) / length(saks)) > AssessmentApi.Web.Lib.Stats.mastery_value, else: false)
            end)
            |> Enum.map(fn {s, _} -> s end)
          {:ok, developing_students}
        end)
      end)
    end

    field :developing_student_ids, non_null(list_of(non_null(:id))) do
      resolve(fn id, _, info ->
        AssessmentApi.Web.Schema.Domain.ObjectiveAnalytics.Helper.get_student_answer_keys_for_objective_by_student(info.context.loader, id, fn (loader, sak_by_student) ->
          developing_students =
            sak_by_student
            |> Enum.filter(fn {student_id, saks} ->
              !(if (length(saks) > 0), do: (((Enum.filter(saks, fn sak -> sak.correct end)) |> length) / length(saks)) > AssessmentApi.Web.Lib.Stats.mastery_value, else: false)
            end)
            |> Enum.map(fn {s, _} -> s end)
          {:ok, developing_students}
        end)
      end)
    end
  end
end
