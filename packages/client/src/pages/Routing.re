type channelId = string;
type seriesId = string;
type measurablesSearchString = string;
type agentId = string;

module AgentPage = {
  module SubPage = {
    type t =
      | AgentMeasurables
      | AgentBots
      | AgentCommunities
      | AgentUpdates
      | AgentScores
      | Unknown;
  };

  type t = {
    agentId,
    subPage: SubPage.t,
  };
};

module ChannelPage = {
  type tab =
    | Measurables
    | Members
    | Options
    | Updates
    | Leaderboard
    | Notebook
    | Notebooks
    | Unknown;

  type leaderboard =
    | ByMeasurable
    | ByMember;

  module SubPage = {
    type t =
      | Measurables(MeasurableQueryIndex.query)
      | Measurable(string)
      | NewMeasurable
      | Members
      | AddMember
      | AddNotebook
      | InviteMember
      | Settings
      | NewSeries
      | Series(seriesId)
      | Notebooks
      | Notebook(string)
      | NotebookDetails(string)
      | FeedItems
      | Leaderboard(leaderboard)
      | Unknown;

    let toTab = (subPage: t): tab =>
      switch (subPage) {
      | Measurables(_) => Measurables
      | Measurable(_) => Measurables
      | NewMeasurable => Measurables
      | NewSeries => Measurables
      | Series(_) => Measurables
      | Members => Members
      | AddMember => Members
      | InviteMember => Members
      | Settings => Options
      | FeedItems => Updates
      | AddNotebook => Notebooks
      | Notebooks => Notebooks
      | Leaderboard(_) => Leaderboard
      | _ => Unknown
      };

    let fromTab = (tab: tab): t =>
      switch (tab) {
      | Measurables => Measurables({state: None})
      | Members => Members
      | Options => Settings
      | Updates => FeedItems
      | Notebooks => Notebooks
      | Leaderboard => Leaderboard(ByMember)
      | _ => Unknown
      };
  };

  type t = {
    channelId,
    subPage: SubPage.t,
  };
};

let getChannelId = (channelId: string): string =>
  switch (channelId) {
  | "home" => ""
  | _ => channelId
  };

module Route = {
  type t =
    | Home
    | Privacy
    | Terms
    | AgentIndex
    | Login
    | Profile
    | Preferences
    | BotCreate
    | BotEdit(string)
    | EntityShow(string)
    | EntityIndex
    | Channel(ChannelPage.t)
    | Agent(AgentPage.t)
    | ChannelIndex
    | ChannelNew
    | MeasurableEdit(string)
    | Subscribe
    | Unsubscribe
    | NotFound;

  let fromUrl = (url: ReasonReact.Router.url) => {
    // @todo: Repair it, we need to move logic from here.
    let showChannel = channelId =>
      Channel({
        channelId: getChannelId(channelId),
        subPage:
          Measurables(
            url.search |> MeasurableQueryIndex.fromStringWithDefaults,
          ),
      });

    switch (url.path) {
    | [] => showChannel("")
    | ["privacy_policy"] => Privacy
    | ["terms_and_conditions"] => Terms
    | ["login"] => Login
    | ["callback"] => showChannel("")
    | ["agents"] => AgentIndex
    | ["profile"] => Profile
    | ["preferences"] => Preferences
    | ["entities"] => EntityIndex
    | ["entities", ...entityId] => EntityShow(String.concat("/", entityId))
    | ["communities", "new"] => ChannelNew
    | ["communities"] => ChannelIndex
    | ["bots", "new"] => BotCreate
    | ["bots", botId, "edit"] => BotEdit(botId)

    // Channels
    | ["c"] => ChannelIndex
    | ["c", channelId] => showChannel(channelId)
    | ["c", channelId, "m", measurableId] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Measurable(measurableId),
      })
    | ["c", channelId, "new"] => Channel({channelId, subPage: NewMeasurable})
    | ["c", channelId, "edit"] => Channel({channelId, subPage: Settings})
    | ["c", channelId, "members"] => Channel({channelId, subPage: Members})
    | ["c", channelId, "activity"] =>
      Channel({channelId: getChannelId(channelId), subPage: FeedItems})
    | ["c", channelId, "scoring", "questions"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Leaderboard(ByMeasurable),
      })
    | ["c", channelId, "scoring", "members"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Leaderboard(ByMember),
      })
    | ["c", channelId, "add"] => Channel({channelId, subPage: AddMember})
    | ["c", channelId, "invite"] =>
      Channel({channelId, subPage: InviteMember})
    | ["c", channelId, "s", "new"] =>
      Channel({channelId, subPage: NewSeries})
    | ["c", channelId, "s", seriesId] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Series(seriesId),
      })

    // Notebooks
    | ["c", channelId, "notebooks"] =>
      Channel({channelId: getChannelId(channelId), subPage: Notebooks})
    | ["c", channelId, "n", notebookId, "details"] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: NotebookDetails(notebookId),
      })
    | ["c", channelId, "n", notebookId] =>
      Channel({
        channelId: getChannelId(channelId),
        subPage: Notebook(notebookId),
      })
    | ["c", channelId, "notebooks", "add"] =>
      Channel({channelId, subPage: AddNotebook})

    // Agents
    | ["agents", agentId, "bots"] => Agent({agentId, subPage: AgentBots})
    | ["agents", agentId, "measurables"] =>
      Agent({agentId, subPage: AgentMeasurables})
    | ["agents", agentId, "communities"] =>
      Agent({agentId, subPage: AgentCommunities})
    | ["agents", agentId, "activity"] =>
      Agent({agentId, subPage: AgentUpdates})
    | ["agents", agentId, "scores"] => Agent({agentId, subPage: AgentScores})

    | ["subscribe"] => Subscribe
    | ["unsubscribe"] => Unsubscribe
    | _ => NotFound
    };
  };
};

module Url = {
  type t =
    | Home
    | Privacy
    | Terms
    | AgentIndex
    | Profile
    | Preferences
    | EntityIndex
    | BotCreate
    | BotEdit(string)
    | EntityShow(string)
    | Agent(AgentPage.t)
    | SeriesShow(string, string)
    | MeasurableShow(string, string)
    | SeriesNew(string)
    | ChannelEdit(string)
    | ChannelMembers(string)
    | ChannelFeedItems(string)
    | ChannelLeaderboard(string, ChannelPage.leaderboard)
    | ChannelAddMember(string)
    | ChannelAddNotebook(string)
    | ChannelInviteMember(string)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | ChannelNotebooks(string)
    | ChannelNotebook(string, string)
    | ChannelNotebookDetails(string, string)
    | MeasurableNew(string)
    | Subscribe
    | Login
    | Unsubscribe
    | Unknown;

  let toString = (r: t) =>
    switch ((r: t)) {
    // Members
    | Profile => "/profile/"
    | Preferences => "/preferences/"
    | Subscribe => "/subscribe"
    | Unsubscribe => "/unsubscribe"

    // Bots
    | BotCreate => "/bots/new"
    | BotEdit(botId) => "/bots/" ++ botId ++ "/edit"
    | EntityIndex => "/entities"
    | EntityShow(entityId) => "/entities/" ++ entityId

    // Agents
    | AgentIndex => "/agents"
    | Agent({agentId, subPage: AgentBots}) => "/agents/" ++ agentId ++ "/bots"
    | Agent({agentId, subPage: AgentMeasurables}) =>
      "/agents/" ++ agentId ++ "/measurables"
    | Agent({agentId, subPage: AgentCommunities}) =>
      "/agents/" ++ agentId ++ "/communities"
    | Agent({agentId, subPage: AgentUpdates}) =>
      "/agents/" ++ agentId ++ "/activity"
    | Agent({agentId, subPage: AgentScores}) =>
      "/agents/" ++ agentId ++ "/scores"
    | Agent({agentId, subPage: Unknown}) =>
      "/agents/" ++ agentId ++ "/measurables"

    // Channels
    | ChannelNew => "/communities/" ++ "new"
    | ChannelIndex => "/communities"
    | ChannelShow(channelId) => "/c/" ++ channelId
    | ChannelEdit(channelId) => "/c/" ++ channelId ++ "/edit"
    | ChannelNotebooks(channelId) => "/c/" ++ channelId ++ "/notebooks"
    | ChannelAddNotebook(channelId) => "/c/" ++ channelId ++ "/notebooks/add"
    | ChannelMembers(channelId) => "/c/" ++ channelId ++ "/members"
    | ChannelFeedItems(channelId) => "/c/" ++ channelId ++ "/activity"
    | ChannelLeaderboard(channelId, ByMeasurable) =>
      "/c/" ++ channelId ++ "/scoring/questions"
    | ChannelLeaderboard(channelId, ByMember) =>
      "/c/" ++ channelId ++ "/scoring/members"
    | ChannelAddMember(channelId) => "/c/" ++ channelId ++ "/add"
    | ChannelInviteMember(channelId) => "/c/" ++ channelId ++ "/invite"
    | ChannelNotebook(channelId, notebookId) =>
      "/c/" ++ channelId ++ "/n/" ++ notebookId
    | ChannelNotebookDetails(channelId, notebookId) =>
      "/c/" ++ channelId ++ "/n/" ++ notebookId ++ "/details"

    // Measurables
    | MeasurableNew(channelId) => "/c/" ++ channelId ++ "/new"
    | MeasurableShow(channelId, measurableId) =>
      "/c/" ++ channelId ++ "/m/" ++ measurableId

    // Series
    | SeriesNew(channelId) => "/c/" ++ channelId ++ "/s/new"
    | SeriesShow(channelId, id) => "/c/" ++ channelId ++ "/s/" ++ id

    // Statis pages
    | Home => "/"
    | Privacy => "/privacy_policy"
    | Terms => "/terms_and_conditions"
    | Login => "/login"
    | Unknown => "/"
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;

  let fromChannelPage = (channelPage: ChannelPage.t) =>
    switch (channelPage.subPage) {
    // Measurables
    | Measurables(_) => ChannelShow(channelPage.channelId)
    | NewMeasurable => MeasurableNew(channelPage.channelId)
    | Measurable(measurableId) =>
      MeasurableShow(channelPage.channelId, measurableId)

    // Other
    | Members => ChannelMembers(channelPage.channelId)
    | Notebooks => ChannelNotebooks(channelPage.channelId)
    | FeedItems => ChannelFeedItems(channelPage.channelId)
    | Leaderboard(subTab) =>
      ChannelLeaderboard(channelPage.channelId, subTab)
    | AddMember => ChannelAddMember(channelPage.channelId)
    | AddNotebook => ChannelAddNotebook(channelPage.channelId)
    | InviteMember => ChannelInviteMember(channelPage.channelId)
    | Settings => ChannelEdit(channelPage.channelId)
    | NewSeries => SeriesNew(channelPage.channelId)
    | Series(seriesId) => SeriesShow(channelPage.channelId, seriesId)
    | _ => Unknown
    };
};