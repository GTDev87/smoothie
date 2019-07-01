let component = ReasonReact.statelessComponent("TestPreview");

let css = Css.css;
let cx = Css.cx;
let tw = Css.tw;

let testPreviewClass = [%bs.raw {| css(tw` mx-32 my-12 `)|}];
let testPreviewHeaderClass = [%bs.raw {| css(tw` w-full flex `)|}];
let testPreviewHeaderSectionClass = [%bs.raw {| css(tw` w-1/3 `)|}];

let titleClass = [%bs.raw {| css(tw` flex justify-center mb-4 `)|}];
let objectiveClass = [%bs.raw {| css(tw` flex justify-center w-full `)|}];
let descriptionClass = [%bs.raw {| css(tw` mb-16`)|}];
let boldWordClass = [%bs.raw {| css(tw` inline font-semibold `)|}];
let objectiveParagraphClass = [%bs.raw {| css(tw` inline `)|}];
let descriptionParagraphClass = [%bs.raw {| css(tw` inline `)|}];

let questionCardSeparatorClass = [%bs.raw {| css(tw` mb-4 `)|}];

let make = (~testId, ~normalized, _children) => {
  ...component,
  render: _self =>
    switch (MyNormalizr.Converter.Test.Remote.getRecord(normalized, testId)) {
    | None => <div />
    | Some(test) =>
      <div className=testPreviewClass>
        <div className=testPreviewHeaderClass>
          <div className=testPreviewHeaderSectionClass>
            <div>{ReasonReact.string("Name:___________________________")}</div>
          </div>
          <div className=cx(testPreviewHeaderSectionClass, titleClass)>
            <h2> {ReasonReact.string(test.data.name)} </h2>
          </div>
          <div className=testPreviewHeaderSectionClass />
        </div>
        <div className=descriptionClass>
          <div className=boldWordClass>
            {ReasonReact.string("Description: ")}
          </div>
          <p className=descriptionParagraphClass>
            {ReasonReact.string(test.data.description)}
          </p>
        </div>
        <div>
          {
            test.data.questionIds
            |> Belt.List.map(_, questionId =>
                 normalized
                 |> MyNormalizr.getQuestionFromSchema(_, questionId)
                 |> Belt.Option.getWithDefault(_, Question.Model.Record.defaultWithId(questionId))
               )
            |> Belt.List.mapWithIndex(_, (idx, question) =>
                 <div className=questionCardSeparatorClass>
                   <QuestionCard normalized number={idx + 1} question blankPlaceholder=true />
                 </div>
               )
            |> Utils.ReasonReact.listToReactArray
          }
        </div>
      </div>
    },
};