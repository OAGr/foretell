open BsReform;

module FormConfig = [%lenses
  type state = {
    name: string,
    description: string,
    competitorType: Types.competitorType,
    picture: string,
  }
];

module Form = ReForm.Make(FormConfig);

type result('a) = ReasonApolloHooks.Mutation.controledVariantResult('a);

let schema =
  Form.Validation.Schema([|
    Custom(
      Name,
      values =>
        Js.String.length(values.name) > 64
          ? ReSchema.Error("Must be less than 64 characters.") : Valid,
    ),
    Custom(
      Name,
      values =>
        Js.String.length(values.name) < 3
          ? Error("Must be over 2 characters.") : Valid,
    ),
  |]);

module FieldString = {
  [@react.component]
  let make = (~field, ~label) => {
    <Form.Field
      field
      render={({handleChange, error, value, validate}) =>
        <Antd.Form.Item label={label |> Utils.ste}>
          <Antd.Input
            value
            onChange={BsReform.Helpers.handleChange(handleChange)}
            onBlur={_ => validate()}
          />
          <Warning error />
        </Antd.Form.Item>
      }
    />;
  };
};

let onSuccess = (loggedUser: Types.user) => {
  Utils.setTimeout(
    _ =>
      Primary.User.getAgent(loggedUser, agent =>
        Routing.Url.push(Agent({agentId: agent.id, subPage: AgentBots}))
      ),
    1000,
  )
  |> ignore;
  ();
};

module FormComponent = {
  [@react.component]
  let make = (~reform: Form.api, ~result: result('a)) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <Sorry />
       | Data(_) => <Spin />
       | _ =>
         <ForetoldComponents.PageCard.BodyPadding>
           <Antd.Form onSubmit>
             <FieldString field=FormConfig.Name label="Name" />
             <FieldString field=FormConfig.Description label="Description" />
             <FieldString field=FormConfig.Picture label="Picture" />
             <Antd.Form.Item>
               {reform.state.formState == Submitting
                  ? <Spin />
                  : <Antd.Button _type=`primary onClick=onSubmit>
                      {"Submit" |> Utils.ste}
                    </Antd.Button>}
             </Antd.Form.Item>
           </Antd.Form>
         </ForetoldComponents.PageCard.BodyPadding>
       }}
    </Form.Provider>;
  };
};

module Create = {
  [@react.component]
  let make = (~loggedUser) => {
    let (mutate, result, _) = BotCreateMutation.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                BotCreateMutation.Query.make(
                  ~input={
                    "name": state.values.name,
                    "description": Some(state.values.description),
                    "competitorType": state.values.competitorType,
                    "picture": Some(state.values.picture),
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"user", "bots"|],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(_) => onSuccess(loggedUser)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={
          name: "",
          description: "",
          competitorType: `COMPETITIVE,
          picture: "",
        },
        (),
      );

    <FormComponent reform result />;
  };
};

module Edit = {
  [@react.component]
  let make = (~bot: Types.bot, ~loggedUser) => {
    let (mutate, result, _) = BotUpdate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                BotUpdate.Query.make(
                  ~id=bot.id,
                  ~input={
                    "name": state.values.name,
                    "description": Some(state.values.description),
                    "competitorType": state.values.competitorType,
                    "picture": Some(state.values.picture),
                  },
                  (),
                )##variables,
              ~refetchQueries=[|"user", "bots"|],
              (),
            )
            |> Js.Promise.then_((result: result('a)) => {
                 switch (result) {
                 | Data(_) => onSuccess(loggedUser)
                 | _ => ()
                 };
                 Js.Promise.resolve();
               })
            |> ignore;

            None;
          },
        ~initialState={
          name: bot.name |> Rationale.Option.default(""),
          description: bot.description |> Rationale.Option.default(""),
          competitorType: `COMPETITIVE,
          picture: bot.picture |> Rationale.Option.default(""),
        },
        (),
      );

    <FormComponent reform result />;
  };
};