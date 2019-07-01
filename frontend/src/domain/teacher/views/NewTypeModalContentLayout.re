let component = ReasonReact.statelessComponent("NewTypeModalContentLayout");

let newContentStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~margin="3em 0",
    (),
  );

let rowStyle = ReactDOMRe.Style.make(~width="100%", ());

let flexCenter =
  ReactDOMRe.Style.make(~display="flex", ~justifyContent="center", ());

let inputContentStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~marginBottom="2em",
    (),
  );

let h1Style = ReactDOMRe.Style.make(~marginBottom="1em", ());

let descriptionTextStyle =
  ReactDOMRe.Style.make(~fontSize=".7em", ~textAlign="center", ());

let make = (~titleText, ~descriptionText, ~buttonText, ~addType, children) => {
  ...component,
  render: _self =>
    <div style=newContentStyle>
      <Row style=rowStyle>
        <Col xs=3 />
        <Col xs=6>
          <div style=flexCenter>
            <h1 style=h1Style> {ReasonReact.string(titleText)} </h1>
          </div>
          <div style=flexCenter>
            <p style=descriptionTextStyle>
              {ReasonReact.string(descriptionText)}
            </p>
          </div>
          <div style=inputContentStyle>
            {
              ReasonReact.createDomElement(
                "div",
                ~props={"style": inputContentStyle},
                children,
              )
            }
          </div>
          <div style=flexCenter>
            <Button onClick={_e => addType()} theme=Button.CTA autoFocus=false>
              {ReasonReact.string(buttonText)}
            </Button>
          </div>
        </Col>
        <Col xs=3 />
      </Row>
    </div>,
};