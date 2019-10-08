module Query = [%graphql
  {|
        mutation botUpdate($id: String!, $input: BotInput!) {
            botUpdate(id: $id, input: $input) {
             id
            }
        }
    |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate =
    (mutation: Mutation.apolloMutation, id, name, description, competitorType) => {
  let m =
    Query.make(
      ~id,
      ~input={
        "name": name,
        "description": Some(description),
        "competitorType": competitorType,
      },
      (),
    );
  mutation(~variables=m##variables, ~refetchQueries=[|"user", "bots"|], ())
  |> ignore;
};

let component = innerComponentFn =>
  <Mutation onError={e => Js.log2("Graphql Error:", e)}>
    ...innerComponentFn
  </Mutation>;
