[@bs.config {jsx: 3}];

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
      | AgentScores;
  };

  type t = {
    agentId,
    subPage: SubPage.t,
  };
};

module NotebookPage = {
  module SubPage = {
    type t =
      | Dashboards
      | Details;
  };

  type t = {
    notebookId: Types.notebookId,
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
    | Dashboard
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
      | InviteMember
      | Settings
      | NewSeries
      | Dashboard
      | Notebooks
      | Series(seriesId)
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
      | Dashboard => Dashboard
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
    | Notebook(NotebookPage.t)
    | ChannelIndex
    | ChannelNew
    | MeasurableEdit(string)
    | Subscribe
    | Unsubscribe
    | NotFound;

  let fromUrl = (url: ReasonReact.Router.url) => {
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
    | ["callback"] =>
      Auth.UrlToAuth0Tokens.make(url)
      |> E.O.fmap(Auth0Tokens.set)
      |> E.O.default();
      showChannel("");
    | ["agents"] => AgentIndex
    | ["profile"] => Profile
    | ["preferences"] => Preferences
    | ["entities"] => EntityIndex
    | ["entities", ...id] => EntityShow(String.concat("/", id))
    | ["communities", "new"] => ChannelNew
    | ["communities"] => ChannelIndex
    | ["bots", "new"] => BotCreate
    | ["bots", id, "edit"] => BotEdit(id)
    | ["measurables", id, "edit"] => MeasurableEdit(id)

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
    | ["c", channelId, "dashboard"] =>
      Channel({channelId: getChannelId(channelId), subPage: Dashboard})
    | ["c", channelId, "notebooks"] =>
      Channel({channelId: getChannelId(channelId), subPage: Notebooks})
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

    // Agents
    | ["agents", agentId, "bots"] => Agent({agentId, subPage: AgentBots})
    | ["agents", agentId, "measurables"] =>
      Agent({agentId, subPage: AgentMeasurables})
    | ["agents", agentId, "communities"] =>
      Agent({agentId, subPage: AgentCommunities})
    | ["agents", agentId, "activity"] =>
      Agent({agentId, subPage: AgentUpdates})
    | ["agents", agentId, "scores"] => Agent({agentId, subPage: AgentScores})

    // Notebooks
    | ["n", notebookId, "dashboards"] =>
      Notebook({notebookId, subPage: Dashboards})
    | ["n", notebookId, "details"] =>
      Notebook({notebookId, subPage: Details})

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
    | Notebook(NotebookPage.t)
    | SeriesShow(string, string)
    | MeasurableShow(string, string)
    | SeriesNew(string)
    | MeasurableEdit(string)
    | ChannelEdit(string)
    | ChannelMembers(string)
    | ChannelFeedItems(string)
    | ChannelLeaderboard(string, ChannelPage.leaderboard)
    | ChannelAddMember(string)
    | ChannelInviteMember(string)
    | ChannelDashboard(string)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | ChannelNotebooks(string)
    | MeasurableNew(string)
    | Subscribe
    | Login
    | Unsubscribe;

  let toString = (r: t) =>
    switch ((r: t)) {
    | Home => "/"
    | Privacy => "/privacy_policy"
    | Terms => "/terms_and_conditions"
    | Profile => "/profile/"
    | Preferences => "/preferences/"
    | BotCreate => "/bots/new"
    | BotEdit(id) => "/bots/" ++ id ++ "/edit"
    | EntityIndex => "/entities"
    | EntityShow(id) => "/entities/" ++ id

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

    // Notebooks
    | Notebook({notebookId, subPage: Dashboards}) =>
      "/n/" ++ notebookId ++ "/dashboards"
    | Notebook({notebookId, subPage: Details}) =>
      "/n/" ++ notebookId ++ "/details"

    // Channels
    | ChannelNew => "/communities/" ++ "new"
    | ChannelIndex => "/communities"
    | ChannelShow(channelId) => "/c/" ++ channelId
    | ChannelEdit(channelId) => "/c/" ++ channelId ++ "/edit"
    | ChannelDashboard(channelId) => "/c/" ++ channelId ++ "/dashboard"
    | ChannelNotebooks(channelId) => "/c/" ++ channelId ++ "/notebooks"
    | ChannelMembers(channelId) => "/c/" ++ channelId ++ "/members"
    | ChannelFeedItems(channelId) => "/c/" ++ channelId ++ "/activity"
    | ChannelLeaderboard(channelId, ByMeasurable) =>
      "/c/" ++ channelId ++ "/scoring/questions"
    | ChannelLeaderboard(channelId, ByMember) =>
      "/c/" ++ channelId ++ "/scoring/members"
    | ChannelAddMember(channelId) => "/c/" ++ channelId ++ "/add"
    | ChannelInviteMember(channelId) => "/c/" ++ channelId ++ "/invite"

    | MeasurableEdit(measurableId) =>
      "/measurables/" ++ measurableId ++ "/edit"
    | MeasurableNew(channelId) => "/c/" ++ channelId ++ "/new"
    | SeriesNew(channelId) => "/c/" ++ channelId ++ "/s/new"
    | SeriesShow(channelId, id) => "/c/" ++ channelId ++ "/s/" ++ id
    | MeasurableShow(channelId, measurableId) =>
      "/c/" ++ channelId ++ "/m/" ++ measurableId
    | Subscribe => "/subscribe"
    | Unsubscribe => "/unsubscribe"
    | Login => "/login"
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;

  let fromChannelPage = (channelPage: ChannelPage.t) =>
    switch (channelPage.subPage) {
    | Measurables(_) => ChannelShow(channelPage.channelId)
    | NewMeasurable => MeasurableNew(channelPage.channelId)
    | Members => ChannelMembers(channelPage.channelId)
    | Dashboard => ChannelDashboard(channelPage.channelId)
    | Notebooks => ChannelNotebooks(channelPage.channelId)
    | FeedItems => ChannelFeedItems(channelPage.channelId)
    | Leaderboard(subTab) =>
      ChannelLeaderboard(channelPage.channelId, subTab)
    | AddMember => ChannelAddMember(channelPage.channelId)
    | InviteMember => ChannelInviteMember(channelPage.channelId)
    | Settings => ChannelEdit(channelPage.channelId)
    | NewSeries => SeriesNew(channelPage.channelId)
    | Series(seriesId) => SeriesShow(channelPage.channelId, seriesId)
    | Measurable(measurableId) =>
      MeasurableShow(channelPage.channelId, measurableId)
    };
};
