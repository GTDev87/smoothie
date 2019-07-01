let component = ReasonReact.statelessComponent("AccountTypeLayout");

let backgroundStyle =
  ReactDOMRe.Style.make(
    ~display="flex",
    ~justifyContent="center",
    ~alignItems="center",
    (),
  );

let buttonsStyle =
  ReactDOMRe.Style.make(
    ~color="white",
    ~display="flex",
    ~justifyContent="space-between",
    (),
  );
let textStyle =
  ReactDOMRe.Style.make(
    ~fontSize="1.5em",
    ~color="white",
    ~display="flex",
    ~justifyContent="center",
    ~marginBottom="1.5em",
    (),
  );

let disableButton =
  ReactDOMRe.Style.make(~cursor="default", ~backgroundColor="gray", ());

let make = _children => {
  ...component,
  render: _self =>
    <LoginBackground style=backgroundStyle>
      <User.Mutation.CreateTeacher id={UUID.generateUUID()}>
        ...{
             (~createTeacher) =>
               <div>
                 <div style=textStyle>
                   {
                     ReasonReact.string(
                       "What kind of account are you creating?",
                     )
                   }
                 </div>
                 <div style=buttonsStyle>
                   <Button
                     theme=CTA
                     autoFocus=false
                     onClick={
                       _ => ApolloType.runApollo(createTeacher) |> ignore
                     }>
                     {ReasonReact.string("Teacher")}
                   </Button>
                   <Button
                     theme=CTA
                     autoFocus=false
                     style=disableButton
                     disabled=true>
                     {ReasonReact.string("Student (Coming Soon)")}
                   </Button>
                 </div>
               </div>
           }
      </User.Mutation.CreateTeacher>
    </LoginBackground>,
};