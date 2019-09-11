open Utils;
open Style.Grid;

module Styles = {
  open Css;
  let paddingRight = [paddingRight(`em(1.))] |> style;
};

let title =
  <Fc.FC.Base.Div float=`left>
    <Fc.FC.PageCard.HeaderRow.Title>
      {"Bots" |> ReasonReact.string}
    </Fc.FC.PageCard.HeaderRow.Title>
  </Fc.FC.Base.Div>;

let agentSection = (agent: AgentGet.agent) =>
  switch (agent) {
  | {user: Some(_user)} =>
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

  let nameColumn: column =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (bot: Types.bot) =>
          switch (bot.name, bot.agent) {
          | (Some(name), Some(agent)) =>
            <Link.Jsx2
              linkType={
                Internal(
                  Agent({agentId: agent.id, subPage: AgentMeasurements}),
                )
              }>
              {name |> ste}
            </Link.Jsx2>
          | _ => ReasonReact.null
          },
      (),
    );

  let descriptionColumn: column =
    Table.Column.make(
      ~name="Description" |> ste,
      ~render=
        (bot: Types.bot) =>
          bot.description |> Rationale.Option.default("") |> ste,
      (),
    );

  let tokenColumn: column =
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

  let editColumn: column =
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

let getUserId = (agent: AgentGet.agent): string => {
  switch (agent.user) {
  | Some(user) => user.id
  | None => ""
  };
};

let make = (~pageParams, ~layout=SLayout.FullPage.makeWithEl, _children) => {
  ...component,
  render: _ =>
    AgentGet.component(
      ~id=pageParams.id,
      ({agent}) => {
        let showBots = bots =>
          Array.length(bots) > 0
            ? <Fc.FC.PageCard.Body>
                {Table.fromColumns(Columns.all, bots, ())}
              </Fc.FC.PageCard.Body>
            : <SLayout.NothingToShow />;

        let body =
          getUserId(agent) !== ""
            ? BotsGet.component(~ownerId=getUserId(agent), showBots)
            : <SLayout.NothingToShow />;

        let head =
          <div>
            title
            <Fc.FC.Base.Div
              float=`right
              className={Css.style([
                Fc.FC.PageCard.HeaderRow.Styles.itemTopPadding,
                Fc.FC.PageCard.HeaderRow.Styles.itemBottomPadding,
              ])}>
              {agentSection(agent)}
            </Fc.FC.Base.Div>
          </div>;

        SLayout.LayoutConfig.make(~head, ~body) |> layout;
      },
    ),
};