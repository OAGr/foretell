module Query = [%graphql
  {|
    mutation userUpdate (
        $id: String!
        $input: UserUpdateInput!
    ) {
        userUpdate (
            id: $id
            input: $input
        ) {
            id
        }
    }
 |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);