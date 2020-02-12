module Styles = {
  open Css;
  let text = style([float(`left)]);
  let icons = style([float(`left), marginLeft(`em(0.5))]);
  let clear = style([clear(`both)]);
};

module Splitter = {
  type block =
    | Text(string)
    | Agent(string);

  type blocks = array(block);

  let regExp = () => [%re "/^>([^\$&]+)<$/"];

  let markdownToBlocks = (str: string): blocks => {
    let splitUp = Js.String.splitByRe([%re "/<agent|\/agent>/"], str);
    splitUp
    |> E.A.fmap(e =>
         switch (e) {
         | Some(str) =>

         let matching = regExp() |> Js.Re.exec(str);
         switch(matching) {
             | None => Some(Text(str))
             | Some(_) => Some(Agent(str))
         };


         | _ => None
         }
       )
    |> E.A.O.concatSome;
  };
};

module Columns = {
  type record = Types.feedItem;
  type column = Table.column(record);

  let name = (r: record) =>
    switch (r.body) {
    | Measurable(row) =>
      <Link
        linkType={Internal(MeasurableShow(r.channelId, row.measurableId))}>
        {row.item |> Utils.ste}
      </Link>
    | Measurement(row) =>
      <Link
        linkType={Internal(MeasurableShow(r.channelId, row.measurableId))}>
        {row.item |> Utils.ste}
      </Link>
    | JoinedMember(row) => row.item |> Utils.ste
    | Channel(row) => row.item |> Utils.ste
    | Notebook(row) => row.item |> Utils.ste
    | _ => "" |> Utils.ste
    };

  let description = (r: record) =>
    switch (r.body) {
    | Generic(row) => row.description |> Utils.ste
    | Measurable(row) => row.description |> Utils.ste
    | Measurement(row) =>
      <>
        <div className=Styles.text> {row.description |> Utils.ste} </div>
        <div className=Styles.text>
          {row.description
           |> Splitter.markdownToBlocks
           |> E.A.fmapi((key, e) =>
                switch (e) {
                | Splitter.Text(str) => "Text: " ++ str ++ ". " |> Utils.ste
                | Splitter.Agent(str) => "Agent: " ++ str ++ ". " |> Utils.ste
                }
              )
           |> ReasonReact.array}
        </div>
        <div className=Styles.icons>
          <MeasurementItems.Info.ById measurementId={row.measurementId} />
        </div>
      </>
    | JoinedMember(row) => row.description |> Utils.ste
    | Channel(row) => row.description |> Utils.ste
    | Notebook(row) =>
      <>
        <div className=Styles.text> {row.description |> Utils.ste} </div>
        <div className=Styles.icons>
          <Link
            className=Style.iconGray
            linkType={Internal(ChannelNotebook(r.channelId, row.notebookId))}>
            <Icon icon="LINK" />
          </Link>
        </div>
      </>
    | _ => "" |> Utils.ste
    };

  let channel =
    Table.Column.make(
      ~name="Community" |> Utils.ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(ChannelShow(r.channelId))}>
            {r.channel.name |> Utils.ste}
          </Link>,
      ~flex=2,
      (),
    );

  let item =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> name,
      ~flex=3,
      (),
    );

  let description =
    Table.Column.make(
      ~name="Description" |> Utils.ste,
      ~render=(r: record) => r |> description,
      ~flex=3,
      (),
    );

  let time =
    Table.Column.make(
      ~name="Time" |> Utils.ste,
      ~render=
        (r: record) =>
          r.createdAt
          |> E.O.fmap((createdAt: MomentRe.Moment.t) =>
               createdAt |> MomentRe.Moment.fromNow(~withoutSuffix=None)
             )
          |> E.O.default("")
          |> Utils.ste,
      ~flex=1,
      (),
    );

  let all = [|channel, item, description, time|];
  let short = [|item, description, time|];
};

[@react.component]
let make = (~feedItems, ~columns=Columns.all) =>
  <Table columns rows=feedItems />;