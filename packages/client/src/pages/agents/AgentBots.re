open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let paddingRight = [paddingRight(`em(1.))] |> style;
};

let title =
  <FC.Base.Div.Jsx2 float=`left>
    <FC.PageCard.HeaderRow.Title>
      {"Bots" |> ReasonReact.string}
    </FC.PageCard.HeaderRow.Title>
  </FC.Base.Div.Jsx2>;

let agentSection = (agent: Types.agent) =>
  switch (agent.agentType) {
  | Some(User(_)) =>
    <>
      {E.React.showIf(
         agent.isMe,
         <Div float=`right>
           <Antd.Button
             onClick={_ => Routing.Url.push(BotCreate)} _type=`primary>
             {"New Bot" |> ste}
           </Antd.Button>
         </Div>,
       )}
    </>
  | _ => E.React.null
  };

let component = ReasonReact.statelessComponent("AgentBots");

module Columns = {
  type column = Table.column(Types.bot);

  let nameColumn =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (bot: Types.bot) =>
          switch (bot.name, bot.agent) {
          | (Some(name), Some(agent)) =>
            <Link.Jsx2
              linkType={
                Internal(Agent({agentId: agent.id, subPage: AgentUpdates}))
              }>
              <AgentLink.Jsx2 agent />
            </Link.Jsx2>
          | _ => ReasonReact.null
          },
      (),
    );

  let descriptionColumn =
    Table.Column.make(
      ~name="Description" |> ste,
      ~render=
        (bot: Types.bot) =>
          bot.description |> Rationale.Option.default("") |> ste,
      (),
    );

  let tokenColumn =
    Table.Column.make(
      ~name="Token" |> ste,
      ~render=
        (bot: Types.bot) =>
          <div className=Styles.paddingRight>
            <Antd.Input value={bot.token |> Rationale.Option.default("")} />
          </div>,
      ~show=(bot: Types.bot) => bot.token != None,
      (),
    );

  let editColumn =
    Table.Column.make(
      ~name="Edit" |> ste,
      ~render=
        (bot: Types.bot) =>
          <Link.Jsx2 linkType={Internal(BotEdit(bot.id))}>
            {"Edit Bot" |> ste}
          </Link.Jsx2>,
      ~show=
        (bot: Types.bot) =>
          Primary.Permissions.can(`BOT_UPDATE, bot.permissions),
      (),
    );

  let all: array(column) = [|
    nameColumn,
    descriptionColumn,
    tokenColumn,
    editColumn,
  |];
};

type pageParams = {id: string};

let make = (~pageParams, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    AgentGet.component(~id=pageParams.id, agent =>
      switch (agent) {
      | Success(Some(agent)) =>
        let showBots = bots =>
          Array.length(bots) > 0
            ? <FC.PageCard.Body>
                {Table.fromColumns(Columns.all, bots, ())}
              </FC.PageCard.Body>
            : <NothingToShow />;

        let body =
          switch (agent.agentType) {
          | Some(User(user)) =>
            BotsGet.component(~ownerId=user.id, showBots)
          | _ => <NothingToShow />
          };

        let head =
          <div>
            title
            <FC.Base.Div.Jsx2
              float=`right
              className={Css.style([
                FC.PageCard.HeaderRow.Styles.itemTopPadding,
                FC.PageCard.HeaderRow.Styles.itemBottomPadding,
              ])}>
              {agentSection(agent)}
            </FC.Base.Div.Jsx2>
          </div>;

        SLayout.LayoutConfig.make(~head, ~body) |> layout;
      | _ => <SLayout> <NothingToShow /> </SLayout>
      }
    ),
};
