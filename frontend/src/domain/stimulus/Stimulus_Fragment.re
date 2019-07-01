module GraphFragment = [%graphql
  {|
    fragment stimulusFields on Stimulus {
      id
      text
    }
  |}
];

include GraphFragment;
module Fields = GraphFragment.StimulusFields;

let fragmentType = "Stimulus";