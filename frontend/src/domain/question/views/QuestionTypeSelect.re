let component = ReasonReact.statelessComponent("QuestionTypeSelect");

let fullWidthStyle = ReactDOMRe.Style.make(~width="100%", ());

let questionButtonStyle = active =>
  ReactDOMRe.Style.make(
    ~marginRight=".2em",
    ~backgroundColor=active ? "gray" : "#d3d3d3",
    ~padding=".2em 1em",
    ~color="white",
    ~cursor="pointer",
    (),
  );

let flexStart =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="flex-start", ());

let make =
    (
      ~questionId,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      ~updateQuestionType,
      _children,
    ) => {
  ...component,
  render: _self => {
    let question =
      normalized
      |> MyNormalizr.getQuestionFromSchema(_, questionId)
      |> Belt.Option.getWithDefault(
           _,
           Question.Model.Record.defaultWithId(questionId),
         );
    let questionBaseId = Question_Utils.questionToBaseId(question);

    /* TODO for some reason question comes through as the wrong type */
    /* TODO Question.Model.questionToString(question) */

    let questionBase =
      normalized
      |> MyNormalizr.Converter.QuestionBase.Remote.getRecord(_, questionBaseId)
      |> Belt.Option.getWithDefault(_, QuestionBase.Model.Record.defaultWithId((), questionBaseId));

    let updateQuestion = action =>
      MyNormalizr.Converter.QuestionBase.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        questionBaseId,
        action,
      )
      |> updateNormalizr;

    let updateQuestionToType = (questionType, _) =>
      updateQuestion(
        QuestionBase.Action.ApolloUpdateQuestion(
          () => updateQuestionType(~id=questionBase.data.id, ~questionType),
        ),
      )
      |> ignore;
    <div style=fullWidthStyle>
      <div style=flexStart>
        {
          QuestionType.all()
          |> Belt.List.map(_, questionType =>
               <div
                 key={QuestionType.questionTypeToString(questionType)}
                 style={
                   questionButtonStyle(
                     QuestionType.questionTypeToString(questionType)
                     == QuestionType.questionTypeToString(questionBase.data.questionType),
                   )
                 }
                 onClick={updateQuestionToType(questionType)}>
                 {
                   questionType
                   |> QuestionType.questionTypeToString
                   |> ReasonReact.string
                 }
               </div>
             )
          |> Utils.ReasonReact.listToReactArray
        }
      </div>
    </div>;
  },
};