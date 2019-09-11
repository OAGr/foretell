let component = ReasonReact.statelessComponent("MeasurableIndex");
let make =
    (
      ~channelId: string,
      ~searchParams: MeasurableQueryIndex.query,
      ~loggedInUser: Types.user,
      _children,
    ) => {
  ...component,
  render: _ => {
    module Reducer =
      PaginationFunctor.Make(MeasurableIndex__Logic.ReducerConfig);
    <Reducer
      callFnParams={channelId, states: searchParams.state}
      subComponent={reducerParams =>
        ChannelGet.component2(~id=channelId, channelQuery =>
          SeriesCollectionGet.component2(~channelId, seriesQuery =>
            MeasurablesStateStatsGet.component2(
              ~channelId, measurablesStateStatsQuery =>
              MeasurableIndex__Components.toLayoutInput(
                reducerParams.send,
                searchParams,
                measurablesStateStatsQuery,
                MeasurableIndex__Logic.make({
                  reducerParams,
                  loggedInUser,
                  channelQuery,
                  seriesQuery,
                }),
              )
            )
          )
        )
      }
    />;
  },
};