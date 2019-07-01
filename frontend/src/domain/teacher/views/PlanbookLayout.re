

let component = ReasonReact.statelessComponent("PlanbookLayout");

let css = Css.css;
let tw = Css.tw;

let playbookLayoutClass = [%bs.raw {| css(tw`
  w-full
  h-full
  px-4
`)|}
];

let flexCenterClass = [%bs.raw {| css(tw`
  flex
  justify-center
  items-center
`)|}
];

let planbookLayoutHelloTextClass = [%bs.raw
  {| css(tw`
    w-full
    h-full
    flex
    justify-center
    items-center
    text-blue-light
  `)|}
  ];

let planbookLayoutUserCircleClass = [%bs.raw
  {| css(tw`
    w-full
    h-full
    text-blue-light
    px-24
    sm:px-0
    sm:pr-4
  `)|}
];

let planbookLayoutRowClass = [%bs.raw
  {| css(tw`
    flex
    flex-wrap
    w-full
    pb-12
  `)|}
];

let planbookLayoutColumnClass = [%bs.raw
  {| css(tw`
    w-full
    sm:w-1/6
    px-4
    sm:px-0
    sm:pr-4
  `)|}
];

let make =
    (
      ~teacher: Teacher.Model.Record.t,
      ~normalized,
      ~updateNormalizr:
         Js.Promise.t(MyNormalizr.FullReduced.normalizedType) => Js.Promise.t('a),
      _children,
    ) => {
  ...component,
  render: _self => {
    <div className=playbookLayoutClass>
      <div>
        <ContentHeader> <h1> {ReasonReact.string("Planbook")} </h1> </ContentHeader>
      </div>
      <div className=planbookLayoutRowClass>
        <div className=planbookLayoutColumnClass>
          <UserCircleIcon className=planbookLayoutUserCircleClass />
        </div>
        <div className=planbookLayoutColumnClass>
          <div className=planbookLayoutHelloTextClass>
            <div>
              <h3 className=flexCenterClass> {ReasonReact.string("Welcome")} </h3>
            </div>
          </div>
        </div>
        <div className=planbookLayoutColumnClass />
        <div className=planbookLayoutColumnClass>
          <TeacherCard
            title="TOTAL CLASSES"
            number={Belt.List.length(teacher.data.classroomIds)}
            textString=""
          />
        </div>
        <div className=planbookLayoutColumnClass>
          <TeacherCard
            title="TOTAL STUDENTS"
            number={
              teacher.data.classroomIds
              |> MyNormalizr.Converter.Classroom.idListToFilteredItems(
                   _,
                   MyNormalizr.Converter.Classroom.Remote.getRecord(
                     normalized,
                   ),
                 )
              |> Belt.List.reduce(_, 0, (memo, classroom) =>
                   memo + Belt.List.length(classroom.data.studentIds)
                 )
            }
            textString=""
          />
        </div>
        <div className=planbookLayoutColumnClass>
          <TeacherCard
            title="TOTAL TESTS"
            number={Belt.List.length(teacher.data.testIds)}
            textString=""
          />
        </div>
      </div>
      <PlanbookTestsOverview data=teacher normalized updateNormalizr/>
    </div>
  },
};
