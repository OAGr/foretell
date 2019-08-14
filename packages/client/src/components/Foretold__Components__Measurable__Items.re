open Utils;
open Primary;
open ReactMarkdown;

module Shared = Foretold__Components__Shared;

type measurable = Measurable.t;

let formatDate = e =>
  e |> E.O.fmap(E.M.format(E.M.format_simple)) |> E.O.default("");

let dateOnStyle =
  Css.(
    style([
      marginLeft(`em(0.2)),
      lineHeight(`em(1.56)),
      fontSize(`em(1.1)),
    ])
  );

let dateItem = (~m: measurable, ~showOn=true, ~onStyle=dateOnStyle, ()) =>
  switch (m.labelOnDate |> E.O.fmap(E.M.goFormat_simple), showOn) {
  | (None, _) => None
  | (Some(e), true) =>
    Some(
      [|
        <span className=onStyle key="on"> {"on " |> ste} </span>,
        <span className=Shared.TagLink.dateItem key="dateItem">
          {e |> ste}
        </span>,
      |]
      |> ReasonReact.array,
    )
  | (Some(e), false) =>
    Some(<span className=Shared.TagLink.dateItem> {e |> ste} </span>)
  };

let link = (~m: measurable) => {
  open Css;
  let name = style([fontSize(`em(1.)), color(`hex("333"))]);
  <>
    {MeasurableEntityLinks.nameEntityLink(~m, ~className=Shared.TagLink.item)
     |> E.O.React.defaultNull}
    {MeasurableEntityLinks.propertyEntityLink(
       ~m,
       ~className=Shared.TagLink.property,
     )
     |> E.O.React.defaultNull}
    <span className=name> {m.name |> ste} </span>
    {dateItem(~m, ()) |> E.O.React.defaultNull}
  </>;
};

let description = (~m: measurable) =>
  switch (m.labelCustom) {
  | Some("")
  | None => None
  | Some(text) => Some(<Markdown source=text />)
  };

let endpointResponse = (~m: measurable) =>
  switch (
    m.resolutionEndpoint |> E.O.default(""),
    m.resolutionEndpointResponse,
  ) {
  | ("", _) => None
  | (_, Some(r)) =>
    Some(
      "Current Endpoint Value: " ++ E.Float.with3DigitsPrecision(r) |> ste,
    )
  | _ => None
  };

let questionLink = (~m: measurable) =>
  <Link.Jsx2
    className=Shared.Item.item
    linkType={Internal(MeasurableShow(m.channelId, m.id))}>
    {"Link to This Question" |> Utils.ste}
  </Link.Jsx2>;

let creatorLink = (~m: measurable) =>
  m.creator
  |> E.O.fmap((c: Agent.t) =>
       <Link.Jsx2
         linkType={
           Internal(Agent({agentId: c.id, subPage: AgentMeasurements}))
         }
         className=Shared.Item.item>
         {c.name |> E.O.default("") |> ste}
       </Link.Jsx2>
     );

let editLink = (~m: measurable) =>
  <div className=Shared.Item.item>
    <Link.Jsx2
      linkType={Internal(MeasurableEdit(m.id))}
      className={Shared.Item.itemButton(NORMAL)}>
      {"Edit" |> ste}
    </Link.Jsx2>
  </div>;

let channelLink = (~m: measurable) =>
  <div className=Shared.Item.item>
    <Link.Jsx2
      linkType={Internal(ChannelShow(m.channelId))}
      className=Shared.Item.item>
      {switch (m.channel) {
       | Some(channel) => "#" ++ channel.name |> ste
       | _ => "#channel" |> ste
       }}
    </Link.Jsx2>
  </div>;

let measurements = (~m: measurable) =>
  switch (m.measurementCount) {
  | Some(0) => None
  | None => None
  | Some(count) =>
    let popoverContent =
      "Number of Predictions: " ++ string_of_int(count) |> ste;
    Some(
      <AntdPopover content=popoverContent trigger=`hover placement=`top>
        <div className=Shared.Item.item>
          <Icon.Icon icon="BULB" />
          {count |> string_of_int |> ste}
        </div>
      </AntdPopover>,
    );
  };

let measurers = (~m: measurable) =>
  switch (m.measurerCount) {
  | Some(0) => None
  | None => None
  | Some(count) =>
    Some(
      <div className=Shared.Item.item>
        <Icon.Icon icon="PEOPLE" />
        {count |> string_of_int |> ste}
      </div>,
    )
  };

let id = (~m: measurable, ()) =>
  <div className=Shared.Item.id>
    {"ID:  " ++ m.id |> ReasonReact.string}
  </div>;

let series = (~m: measurable, ~channelId=None, ()) => {
  m.series
  |> E.O.bind(_, r =>
       switch (r.name) {
       | Some(name) =>
         Some(
           <div className=Shared.Item.item>
             <Link.Jsx2 linkType={Internal(SeriesShow(m.channelId, r.id))}>
               <Icon.Icon icon="LAYERS" />
               {name |> ste}
             </Link.Jsx2>
           </div>,
         )
       | None => None
       }
     );
};

let expectedResolutionDate = (~m: measurable) =>
  <div className=Shared.Item.item>
    {"Resolves on " ++ (m.expectedResolutionDate |> formatDate) |> ste}
  </div>;

let resolutionEndpoint = (~m: measurable) =>
  switch (m.resolutionEndpoint |> E.O.default("")) {
  | "" => None
  | text =>
    Some(
      <div className=Shared.Item.item> {"Endpoint: " ++ text |> ste} </div>,
    )
  };

let archiveButton = (~m: measurable) =>
  MeasurableArchive.Mutation.make((mutation, _) =>
    <div className=Shared.Item.item>
      <div
        className={Shared.Item.itemButton(DANGER)}
        onClick={e => {
          MeasurableArchive.mutate(mutation, m.id);
          ReactEvent.Synthetic.stopPropagation(e);
        }}>
        {"Archive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let unArchiveButton = (~m: measurable) =>
  MeasurableUnarchive.Mutation.make((mutation, _) =>
    <div className=Shared.Item.item>
      <div
        className={Shared.Item.itemButton(DANGER)}
        onClick={e => {
          MeasurableUnarchive.mutate(mutation, m.id);
          ReactEvent.Synthetic.stopPropagation(e);
        }}>
        {"Unarchive" |> ste}
      </div>
    </div>
  )
  |> E.React.el;

let archiveOption = (~m: measurable) =>
  m.isArchived == Some(true) ? unArchiveButton(~m) : archiveButton(~m);