let component = ReasonReact.statelessComponent("Tooltip");

let make = (~tooltipText: string, children) => {
  ...component,
  render: _self => {
    let randomString = Random.randomString();

    Document.isBrowser() ?
      <div>
        {
          ReasonReact.createDomElement(
            "div",
            ~props={"id": randomString},
            children,
          )
        }
        <ReactTooltip target=randomString placement="right">
          {ReasonReact.string(tooltipText)}
        </ReactTooltip>
      </div> :
      ReasonReact.createDomElement("div", ~props=Js.Obj.empty(), children);
  },
};