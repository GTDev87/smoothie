defmodule AssessmentApi.Web.Schema.Member.Mutations do
  use Absinthe.Schema.Notation
  import_types(AssessmentApi.Web.Schema.Member.Mutations.User)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Teacher)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Classroom)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Student)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Test)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Objective)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Question)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.Stimulus)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.StudentTest)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.StudentQuestion)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.QuestionAnswerKey)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.StudentAnswerKey)
  import_types(AssessmentApi.Web.Schema.Member.Mutations.MultipleChoiceQuestion)

  object :member_mutation do
    import_fields(:user_member_mutations)
    import_fields(:teacher_member_mutations)
    import_fields(:classroom_member_mutations)
    import_fields(:student_member_mutations)
    import_fields(:test_member_mutations)
    import_fields(:objective_member_mutations)
    import_fields(:question_member_mutations)
    import_fields(:stimulus_member_mutations)
    import_fields(:student_test_member_mutations)
    import_fields(:student_question_member_mutations)
    import_fields(:question_answer_key_member_mutations)
    import_fields(:student_answer_key_member_mutations)
    import_fields(:multiple_choice_question_member_mutations)
  end
end
