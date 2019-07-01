let component = ReasonReact.statelessComponent("QuestionAnswerKeyLayout");
let css = Css.css;
let tw = Css.tw;
let questionAnswerKeyLayoutClass = [%bs.raw {| css(tw` py-2 w-full flex flex-wrap text-black`)|}];
let questionAnswerKeyLayoutTextWrapperClass = [%bs.raw {| css(tw` w-1/3 flex justify-center items-center `)|}];

let make = (
  ~studentAnswerKey: StudentAnswerKey.Model.Record.t,
  ~normalized,
  ~updateNormalizr:
      Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
  ~updateNormalizedWithReducer,
  _children,
) => {
  ...component,
  render: _self => {
    let answerKey =
      normalized
      |> MyNormalizr.Converter.QuestionAnswerKey.Remote.getRecord(_, studentAnswerKey.data.originalId)
      |> Belt.Option.getWithDefault(
        _, QuestionAnswerKey.Model.Record.defaultWithId((), studentAnswerKey.data.originalId));

    <div className=questionAnswerKeyLayoutClass>
      <div className=questionAnswerKeyLayoutTextWrapperClass>
        {answerKey.data.score |> string_of_float |> ReasonReact.string}
      </div>
      <div className=questionAnswerKeyLayoutTextWrapperClass>
        {
          answerKey.data.objectiveId
          |> Belt.Option.mapWithDefault(_, "default", objectiveId =>
              normalized
              |> MyNormalizr.Converter.Objective.Remote.getRecord(_, objectiveId)
              |> Belt.Option.mapWithDefault(_, "default", objective => objective.data.text))
          |> ReasonReact.string
        }
      </div>
      <div className=questionAnswerKeyLayoutTextWrapperClass>
        {
          let updateStudentAnswerKey = action =>
            normalized
            |> Js.Promise.resolve
            |> updateNormalizedWithReducer
            |> MyNormalizr.Converter.StudentAnswerKey.Remote.updateWithDefault(
                 answerKey.data.id,  _, Schema.StudentAnswerKey.stringToId(studentAnswerKey.data.id), action)
            |> updateNormalizr;

          <StudentAnswerKey.Mutation.UpdateStudentAnswerKey>
            ...{(~mutation) =>
              <Checkbox
                value={studentAnswerKey.data.correct}
                onToggle={
                  () =>
                    updateStudentAnswerKey(
                      StudentAnswerKey.Action.ApolloStudentAnswerKey(
                        () =>
                          mutation(
                            ~id=studentAnswerKey.data.id,
                            ~correct=!studentAnswerKey.data.correct,
                          ),
                      ),
                    )
                }
              />
            }
          </StudentAnswerKey.Mutation.UpdateStudentAnswerKey>;
        }
      </div>
    </div>
  }
};