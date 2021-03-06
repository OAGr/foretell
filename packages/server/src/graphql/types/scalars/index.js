const { GraphQLInputInt } = require('graphql-input-number');
const graphql = require('graphql');

const cursor = require('./cursor');
const jwt = require('./jwt');
const { createObjectId } = require('./object-id');
const { createObjectIdEmptiable } = require('./object-id-emptiable');
const { createLimitedString } = require('./limited-string');
const { voteAmount } = require('./vote-amount');

const agentId = createObjectId('AgentId');
const bookmarkId = createObjectId('BookmarkId');
const botId = createObjectId('BoteId');
const channelId = createObjectIdEmptiable('ChannelId');
const channelMembershipId = createObjectId('ChannelMembershipId');
const feedItemId = createObjectId('FeedItemId');
const globalSettingId = createObjectId('GlobalSettingId');
const invitationId = createObjectId('InvitationId');
const measurableId = createObjectId('MeasurableId');
const measurementId = createObjectId('MeasurementId');
const mutexId = createObjectId('MutexId');
const notebookId = createObjectId('NotebookId');
const notificationId = createObjectId('NotificationId');
const notificationStatusId = createObjectId('NotificationStatusId');
const preferenceId = createObjectId('PreferenceId');
const seriesId = createObjectId('SeriesId');
const templateId = createObjectId('TemplateId');
const tokenId = createObjectId('TokenId');
const userId = createObjectId('UserId');
const voteId = createObjectId('VoteId');

// @todo: These "scalars" for migration to upper ^ scalars.
const $agentId = graphql.GraphQLString;
const $bookmarkId = graphql.GraphQLString;
const $botId = graphql.GraphQLString;
const $channelId = graphql.GraphQLString;
const $channelMembershipId = graphql.GraphQLString;
const $feedItemId = graphql.GraphQLString;
const $globalSettingId = graphql.GraphQLString;
const $invitationId = graphql.GraphQLString;
const $measurableId = graphql.GraphQLString;
const $measurementId = graphql.GraphQLString;
const $mutexId = graphql.GraphQLString;
const $notebookId = graphql.GraphQLString;
const $notificationId = graphql.GraphQLString;
const $notificationStatusId = graphql.GraphQLString;
const $preferenceId = graphql.GraphQLString;
const $seriesId = graphql.GraphQLString;
const $templateId = graphql.GraphQLString;
const $tokenId = graphql.GraphQLString;
const $userId = graphql.GraphQLString;
const $voteId = graphql.GraphQLString;
const $agentChannelId = graphql.GraphQLString;
const $cursor = graphql.GraphQLString;
const $globalSettingsName = graphql.GraphQLString;
const $agentMeasurableId = graphql.GraphQLString;

const string0to255 = createLimitedString(0, 255);
const string3to255 = createLimitedString(3, 255);

const string0to512 = createLimitedString(0, 512);
const string3to512 = createLimitedString(3, 512);

const string0to4K = createLimitedString(0, 4 * 1024);
const string3to4K = createLimitedString(3, 4 * 1024);

const string0to32K = createLimitedString(0, 32 * 1024);
const string3to32K = createLimitedString(3, 32 * 1024);

const string0to128K = createLimitedString(0, 128 * 1024);
const string3to128K = createLimitedString(3, 128 * 1024);

const int500 = GraphQLInputInt({
  name: 'Int500',
  min: 1,
  max: 500,
});

module.exports = {
  cursor,
  jwt,
  voteAmount,

  agentId,
  bookmarkId,
  botId,
  channelId,
  channelMembershipId,
  feedItemId,
  globalSettingId,
  invitationId,
  measurableId,
  measurementId,
  mutexId,
  notebookId,
  notificationId,
  notificationStatusId,
  preferenceId,
  seriesId,
  templateId,
  tokenId,
  userId,
  voteId,

  $agentId,
  $bookmarkId,
  $botId,
  $channelId,
  $channelMembershipId,
  $feedItemId,
  $globalSettingId,
  $invitationId,
  $measurableId,
  $measurementId,
  $mutexId,
  $notebookId,
  $notificationId,
  $notificationStatusId,
  $preferenceId,
  $seriesId,
  $templateId,
  $tokenId,
  $userId,
  $voteId,
  $agentChannelId,
  $cursor,
  $globalSettingsName,
  $agentMeasurableId,

  string0to255,
  string3to255,

  string0to512,
  string3to512,

  string0to4K,
  string3to4K,

  string0to32K,
  string3to32K,

  string0to128K,
  string3to128K,

  int500,
};
