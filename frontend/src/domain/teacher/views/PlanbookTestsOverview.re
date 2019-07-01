

let component = ReasonReact.statelessComponent("PlanbookTestsOverview");

let css = Css.css;
let tw = Css.tw;
let planbookTestsOverviewTestRowClass = [%bs.raw
  {| css(tw`
    w-full
    border-0
    border-b
    border-blue-light
    border-solid
  `)|}
];

let make =
    (
      ~data as teacher: Teacher.Model.Record.t,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self => {
    let updateTeacher = action =>
      MyNormalizr.Converter.Teacher.Remote.updateWithDefault(
        (),
        normalized |> Js.Promise.resolve,
        Schema.Teacher.stringToId(teacher.data.id),
        action,
      )
      |> updateNormalizr;

    <InfoCard header=(<TestRowHeader/>)>
      {
        Belt.List.length(teacher.data.testIds) !== 0 ?
          <div>
            {
              teacher.data.testIds
              |> MyNormalizr.Converter.Test.idListToFilteredItems(
                  _, MyNormalizr.Converter.Test.Remote.getRecord(normalized))
              |> Belt.List.map(_, test =>
                  <div className=planbookTestsOverviewTestRowClass>
                    <TestRow
                      key={test.data.id}
                      data=test
                      onClick={
                        () =>
                          updateTeacher(
                            Teacher.Action.SelectSideBar(
                              SideTab.Test,
                              test.data.id,
                            ),
                          )
                      }
                      normalized
                    />
                  </div>
                )
              |> Utils.ReasonReact.listToReactArray
            }
          </div> :
          <div />
      }
    </InfoCard>
  }
};