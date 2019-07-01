defmodule AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper do
  alias AssessmentApi.Web

  def get_student_answer_keys(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.StudentQuestion, :student_test_id, fn (loader, student_questions) ->
      student_question_ids = student_questions |> Enum.map(fn q -> q.id end)
      Web.Lib.BatchUtils.batch_through_field(loader, student_question_ids, Web.Model.StudentAnswerKey, :student_question_id, fn (loader, student_answer_keys) ->
        callback.(loader, student_answer_keys |> Enum.sort(&(NaiveDateTime.compare(&1.inserted_at, &2.inserted_at) == :lt)))
      end)
    end)
  end

  def score(loader, id, callback) do
    get_student_answer_keys(loader, id, fn (loader, student_answer_keys) ->
      score =
        student_answer_keys
        |> Enum.map(fn sak -> (if sak.correct, do: 1, else: 0) end)
        |> Enum.sum
      callback.(loader, score)
    end)
  end

  def test_to_classroom_and_test_ids(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.StudentTest, :id, fn (loader, student_tests) ->
      test_id = student_tests |> Enum.map(fn q -> q.original_id end) |> List.first
      student_id = student_tests |> Enum.map(fn q -> q.student_id end) |> List.first
      Web.Lib.BatchUtils.batch_through_field(loader, student_id, Web.Model.StudentClassroom, :student_id, fn (loader, student_classrooms) ->
        classroom_id =
          student_classrooms
          |> Enum.map(fn q -> q.classroom_id end)
          |> List.first

        callback.(loader, classroom_id, test_id)
      end)
    end)
  end

  def test_to_classroom_test(loader, id, callback) do
    test_to_classroom_and_test_ids(loader, id, fn (loader, classroom_id, test_id) ->
      Web.Lib.BatchUtils.batch_through_field(loader, classroom_id, Web.Model.ClassroomTest, :classroom_id, fn (loader, classroom_tests) ->
        classroom_test =
          classroom_tests
          |> Enum.filter(fn ct -> ct.test_id == test_id end)
          |> List.first

        callback.(loader, classroom_test)
      end)
    end)
  end

  # def get_deviation(loader, id, callback) do
  #   test_to_classroom_test(loader, id, fn (loader, classroom_test) ->

  #     classroom_test_id = classroom_test.id
  #     Web.Schema.Domain.TestAnalytics.Helper.get_scores(loader, classroom_test_id, fn (loader, scores) ->

  #       stdev = scores |> Statistics.stdev
  #       mean = scores |> Statistics.mean

  #       AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.score(loader, id, fn (loader, score) ->
  #         deviation = score - (mean || 0)
  #         callback.(loader, deviation)
  #       end)
  #     end)
  #   end)
  # end

  # def get_deviation_sq(loader, id, callback) do
  #   get_deviation(loader, id, fn (loader, deviation) ->
  #     callback.(loader, deviation * deviation)
  #   end)
  # end

  def get_prop(loader, id, callback) do
    Web.Lib.BatchUtils.batch_through_field(loader, id, Web.Model.StudentTest, :id, fn (loader, student_tests) ->
      test_id = student_tests |> Enum.map(fn q -> q.original_id end) |> List.first
      AssessmentApi.Web.Schema.Domain.Test.Helper.get_num_questions(loader, test_id, fn (loader, num_questions) ->
        AssessmentApi.Web.Schema.Domain.StudentTestAnalytics.Helper.score(loader, id, fn (loader, score) ->
          callback.(loader, (if num_questions == 0, do: 0, else: score / num_questions))
        end)
      end)
    end)
  end

  def get_dev(loader, id, callback) do
    test_to_classroom_test(loader, id, fn (loader, classroom_test) ->
      get_prop(loader, id, fn (loader, prop) ->
        classroom_test_id = classroom_test.id
        Web.Schema.Domain.TestAnalytics.Helper.get_prop_avg(loader, classroom_test_id, fn (loader, prop_avg) ->
          callback.(loader, prop_avg - prop)
        end)
      end)
    end)
  end

  def get_dev_sq(loader, id, callback) do
    get_dev(loader, id, fn (loader, dev) ->
      callback.(loader, dev * dev)
    end)
  end
end
