type t =
  | Dashboard
  | Classroom
  | Test
;

let all = () => [
  Dashboard,
  Classroom,
  Test,
];

let toString = (sideTabType : t) : string =>
  switch(sideTabType){
  | Dashboard => "dashboard"
  | Classroom => "classroom"
  | Test => "test"
  };

let fromString = (string : string) : t =>
  switch(string){
  | "dashboard" => Dashboard
  | "classroom" => Classroom
  | "test" => Test
  | _ => Dashboard
  };