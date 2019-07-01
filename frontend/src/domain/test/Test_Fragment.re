module GraphFragment = [%graphql
  {|
     fragment testFields on Test {
       id
       notes
       questionIds
       questions {
         ...on LongAnswerQuestion {
           ...LongAnswerQuestion.Model.Fragment.Fields
         }
         ...on MultipleChoiceQuestion {
           ...MultipleChoiceQuestion.Model.Fragment.Fields
         }
         ...on TrueFalseQuestion {
          ...TrueFalseQuestion.Model.Fragment.Fields
         }
         ...on FillInTheBlankQuestion {
           ...FillInTheBlankQuestion.Model.Fragment.Fields
         }
       }
       objectiveIds
       objectives{
         ...Objective.Model.Fragment.ObjectiveFields
       }
       name
       description
     }
   |}
];

include GraphFragment;
module Fields = GraphFragment.TestFields;

let fragmentType = "Test";