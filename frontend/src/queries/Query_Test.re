module M = [%graphql
  {|
    query Test($testIds: [ID!]) {
      tests(filter: {ids: $testIds}) {
        ...Test.Model.Fragment.Fields
      }
    }
  |}
];