[@bs.config {jsx: 3}];

open Rationale.Function.Infix;

module CMutationForm =
  MutationForm.Make({
    type queryType = ChannelUpdate.Query.t;
  });

let component = ReasonReact.statelessComponent("ChannelEdit");

let make = (~channelId: string, _children) => {
  ...component,
  render: _ => {
    let loadChannel = ChannelGet.getChannelByIdAsComponent(~id=channelId);

    let mutationMake =
      ChannelUpdate.Mutation.make(~onCompleted=_ => ()) ||> E.React.el;

    let head =
      <>
        <FC.Base.Div.Jsx2 float=`left>
          <FC.PageCard.HeaderRow.Title>
            {"Edit Community" |> Utils.ste}
          </FC.PageCard.HeaderRow.Title>
        </FC.Base.Div.Jsx2>
        <Providers.AppContext.Consumer>
          ...{({loggedUser}) =>
            switch (loggedUser) {
            | Some(loggedUser) =>
              <FC.Base.Div.Jsx2
                float=`right
                className={Css.style([
                  FC.PageCard.HeaderRow.Styles.itemTopPadding,
                ])}>
                {Primary.User.show(
                   loggedUser,
                   <FC.Base.Button
                     variant=Primary
                     size=Small
                     onClick={e =>
                       LinkType.onClick(Internal(SeriesNew(channelId)), e)
                     }>
                     {"New Series" |> Utils.ste}
                   </FC.Base.Button>,
                 )}
              </FC.Base.Div.Jsx2>
            | _ => ReasonReact.null
            }
          }
        </Providers.AppContext.Consumer>
      </>;

    let form = (mutation, channel: Types.channel) =>
      ChannelForm.Form.make(
        ~onSubmit=
          values =>
            ChannelUpdate.mutate(
              mutation,
              channelId,
              values.state.values.name,
              Some(values.state.values.description),
              values.state.values.isPublic |> E.Bool.fromString,
              values.state.values.isArchived |> E.Bool.fromString,
            ),
        ~initialState={
          name: channel.name,
          description: channel.description |> E.O.default(""),
          isPublic: channel.isPublic |> E.Bool.toString,
          isArchived: channel.isArchived |> E.Bool.toString,
        },
        ~schema=ChannelForm.Form.Validation.Schema([||]),
      )
      ||> E.React.el;

    <SLayout head>
      <FC.PageCard.BodyPadding>
        {loadChannel(
           HttpResponse.fmap(channel =>
             mutationMake((mutation, data) =>
               form(mutation, channel, ({send, state}) =>
                 CMutationForm.showWithLoading(
                   ~result=data.result,
                   ~form=
                     ChannelForm.showForm(
                       ~state,
                       ~send,
                       ~creating=false,
                       ~onSubmit=() => send(ChannelForm.Form.Submit),
                       (),
                     ),
                   ~successMessage="Community edited successfully.",
                   (),
                 )
               )
             )
           )
           ||> HttpResponse.withReactDefaults,
         )}
      </FC.PageCard.BodyPadding>
    </SLayout>;
  },
};
