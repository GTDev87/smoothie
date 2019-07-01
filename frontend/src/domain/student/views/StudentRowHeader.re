let component = ReasonReact.statelessComponent("StudentRowHeader");

let fullWidthStyle =
  ReactDOMRe.Style.make(~width="100%", ~margin="0", ~padding=".5em", ());
let css = Css.css;
let tw = Css.tw;

let rowClass = [%bs.raw
  {| css(tw`flex bg-blue-lighter rounded-sm m-0 p-1 flex-wrap w-full text-black`)|}
];
let sixthWidthClass = [%bs.raw {| css(tw` w-1/6`)|}];

let make =
    (
      ~normalized,
      ~classroom: Classroom.Model.Record.t,
      ~testIds: list(Test.Model.idType),
      ~updateNormalizr,
      ~addStudent,
      ~addingNewStudent,
      _children,
    ) => {
  ...component,
  render: _self => {
    let selectedTest =
      switch (classroom.local.testTab) {
      | None =>
        testIds
        |> Belt.List.head
      | a => a
      };
    let updateClassroom = action =>
      MyNormalizr.Converter.Classroom.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        Schema.Classroom.stringToId(classroom.data.id),
        action,
      )
      |> updateNormalizr;
    <div className=rowClass>
      <div className=sixthWidthClass>
        {
          addingNewStudent ?
            <div /> : <div onClick=addStudent> <PlusIcon /> </div>
        }
      </div>
      <div className=sixthWidthClass>{ReasonReact.string("First Name")}</div>
      <div className=sixthWidthClass>{ReasonReact.string("Last Name")}</div>
      <div className=sixthWidthClass>{ReasonReact.string("Group")}</div>
      <div className=sixthWidthClass>
        <Select
          selectedId=selectedTest
          idToString={a => a |> Belt.Option.mapWithDefault(_, "", (testId) => testId |> Test.Model.getUUIDFromId)}
          selections={
            testIds
            |> MyNormalizr.Converter.Test.idListToFilteredItems(_, MyNormalizr.Converter.Test.Remote.getRecord(normalized))
            |> Belt.List.map(_, test =>
                 (
                   {
                     id: Some(test.data.id |> Test.Model.idToTypedId),
                     text: test.data.name
                   }: Select.selectionType(option(Test.Model.idType))
                 )
               )
          }
          onSelect={
            a =>
              switch(a){
              | Some(Some(a)) => updateClassroom(Classroom.Action.ChangeTestTab(a))
              | _ => updateClassroom(Classroom.Action.NoOpKeyDown)
              }
          }
        />
      </div>
      <div className=sixthWidthClass />
    </div>;
  },
};