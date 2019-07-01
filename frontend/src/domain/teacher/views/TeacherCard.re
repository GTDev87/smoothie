let component = ReasonReact.statelessComponent("TeacherCard");

let teacherCardStyle =
  ReactDOMRe.Style.make(
    ~maxHeight="100%",
    ~maxWidth="100%",
    ~padding="1em",
    ~border="2px solid black",
    (),
  );

let rowStyle = ReactDOMRe.Style.make(~height="100%", ~width="100%", ());

let footerText = ReactDOMRe.Style.make(~width="100%", ~fontSize=".5em", ());

let numberStyle =
  ReactDOMRe.Style.make(~width="100%", ~borderBottom="1px solid black", ());

let make = (~title: string, ~number: int, ~textString: string, _children) => {
  ...component,
  render: _self =>
    <div style=teacherCardStyle>
      <Row style=rowStyle>
        <Col xs=9> {ReasonReact.string(title)} </Col>
        <Col xs=3 />
      </Row>
      <Row style=rowStyle>
        <Col xs=12>
          <h3 style=numberStyle>
            {ReasonReact.string(number |> string_of_int)}
          </h3>
        </Col>
      </Row>
      <Row style=rowStyle>
        <Col xs=12>
          <div style=footerText> {ReasonReact.string(textString)} </div>
        </Col>
      </Row>
    </div>,
};