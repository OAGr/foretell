open Utils;
open Style.Grid;
open Measurable__Index__Logic;

module ReducerParams = Reducer.Reducers.ReducerParams;

type measurablesStateStats =
  E.HttpResponse.t(
    option(Foretold__GraphQL.Queries.MeasurablesStateStats.stats),
  );

module LoadedAndSelected = {
  open Measurable__Index__Logic.LoadedAndSelected;

  let header = (t: t, send: Reducer.Types.send) =>
    <>
      <Div
        float=`left
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.deselectButton(send)}
      </Div>
      <Div
        float=`right
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.paginationItem(t.reducerParams)}
      </Div>
    </>;

  let body = (t: t) =>
    <C.Measurable.FullPresentation
      id={t.selectedMeasurable.id}
      key={t.selectedMeasurable.id}
      loggedInUser={t.loggedInUser}
    />;
};

module LoadedAndUnselected = {
  open Measurable__Index__Logic.LoadedAndUnselected;

  let stateLink = (state, text, num: int, isActive) =>
    <FC.Tab2
      isActive
      number=num
      onClick={
        Foretold__Components__Link.LinkType.onClick(
          Relative(
            Context.QueryParams.MeasurableIndex.make(Some(state))
            |> Context.QueryParams.MeasurableIndex.toUrlParams,
          ),
        )
      }>
      {text |> ste}
    </FC.Tab2>;

  let header =
      (
        t: t,
        stats: measurablesStateStats,
        query: Context.QueryParams.MeasurableIndex.query,
      ) =>
    <Div>
      <Div float=`left>
        {
          switch (stats) {
          | Success(Some(r)) =>
            <Div
              float=`left
              styles=[
                Css.style([
                  FC.PageCard.HeaderRow.Styles.itemTopPadding,
                  FC.PageCard.HeaderRow.Styles.itemBottomPadding,
                ]),
              ]>
              {
                stateLink(
                  `OPEN,
                  "Open",
                  r.openTotal,
                  query.state == Some(`OPEN),
                )
              }
              {
                stateLink(
                  `JUDGEMENT_PENDING,
                  "Pending Resolution",
                  r.pendingTotal,
                  query.state == Some(`JUDGEMENT_PENDING),
                )
              }
              {
                stateLink(
                  `JUDGED,
                  "Closed",
                  r.closedTotal,
                  query.state == Some(`JUDGED),
                )
              }
            </Div>
          | _ => <> </>
          }
        }
      </Div>
      <Div
        float=`right
        styles=[
          Css.style([
            FC.PageCard.HeaderRow.Styles.itemTopPadding,
            FC.PageCard.HeaderRow.Styles.itemBottomPadding,
          ]),
        ]>
        {Reducer.Components.paginationPage(t.reducerParams)}
      </Div>
    </Div>;

  let body = (t: t, send: Reducer.Types.send) => {
    let measurables =
      (
        switch (t.reducerParams.response) {
        | Success(r) => Some(r.edges)
        | _ => None
        }
      )
      |> E.O.toExn("");
    <>
      <C.Measurables.BasicTable
        measurables
        showExtraData=true
        channelId={Some(t.channel.id)}
        onSelect={
          e => Reducer.Components.sendSelectItem(t.reducerParams, e.id)
        }
      />
    </>;
  };
};

module MeasurableIndexDataState = {
  open Measurable__Index__Logic.MeasurableIndexDataState;

  let toLayoutInput =
      (
        send: Reducer.Types.send,
        selectedState: Context.QueryParams.MeasurableIndex.query,
        stats: measurablesStateStats,
        state: state,
      ) =>
    switch (state) {
    | InvalidIndexError(_) =>
      SLayout.LayoutConfig.make(
        ~head=E.React.null,
        ~body="Item Not Valid" |> ste,
      )
    | WithChannelButNotQuery(c) =>
      SLayout.LayoutConfig.make(~head=E.React.null, ~body=<SLayout.Spin />)
    | LoadedAndUnselected(l) =>
      SLayout.LayoutConfig.make(
        ~head=LoadedAndUnselected.header(l, stats, selectedState),
        ~body=LoadedAndUnselected.body(l, send),
      )
    | LoadedAndSelected(l) =>
      SLayout.LayoutConfig.make(
        ~head=LoadedAndSelected.header(l, send),
        ~body=LoadedAndSelected.body(l),
      )
    | WithoutChannel(_) =>
      SLayout.LayoutConfig.make(
        ~head=E.React.null,
        ~body="No channel." |> ste,
      )
    };
};