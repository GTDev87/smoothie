module M = [%graphql
  {|
    query Grades {
      grades {
        id
        name
      }
    }
  |}
];