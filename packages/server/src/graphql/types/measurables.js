const graphql = require('graphql');
const { DateType } = require('graphql-sequelize');

const resolvers = require('../resolvers');

const { measurableValueType } = require('./enums');
const { measurableState } = require('./enums');
const scalars = require('./scalars');
const commonTypes = require('./common');
const permissionsTypes = require('./permissions');
const seriesTypes = require('./series');
const agentsTypes = require('./agents');
const channelsTypes = require('./channels');

const name = {
  type: graphql.GraphQLNonNull(scalars.string3to512),
};
const labelSubject = {
  type: graphql.GraphQLString,
  description: 'Description of a question\'s entity',
};
const labelOnDate = {
  type: DateType.default,
  description: 'Description of a question\'s date',
};
const labelProperty = {
  type: graphql.GraphQLString,
  description: 'Description of a question\'s property',
};
const labelCustom = {
  type: graphql.GraphQLString,
  description: 'Description of a question',
};

const measurable = new graphql.GraphQLObjectType({
  name: 'Measurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    name,
    labelSubject,
    labelOnDate,
    labelProperty,
    labelCustom,
    valueType: { type: measurableValueType },
    state: { type: graphql.GraphQLNonNull(measurableState) },
    stateUpdatedAt: { type: DateType.default },
    isArchived: { type: graphql.GraphQLNonNull(graphql.GraphQLBoolean) },
    resolutionEndpoint: { type: graphql.GraphQLString },
    expectedResolutionDate: { type: DateType.default },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },

    measurementCount: {
      type: graphql.GraphQLInt,
      resolve: resolvers.measurements.measurementCountByMeasurableId,
    },

    measurerCount: {
      type: graphql.GraphQLInt,
      resolve: resolvers.measurements.measurerCount,
    },

    resolutionEndpointResponse: {
      type: graphql.GraphQLFloat,
      resolve: resolvers.measurables.resolutionEndpointResponse,
    },

    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    creatorId: { type: graphql.GraphQLString },
    seriesId: { type: graphql.GraphQLString },
    iAmOwner: commonTypes.iAmOwner,
    min: { type: graphql.GraphQLFloat },
    max: { type: graphql.GraphQLFloat },

    permissions: {
      type: graphql.GraphQLNonNull(permissionsTypes.permissions),
      resolve: resolvers.permissions.measurablesPermissions,
    },

    series: {
      type: seriesTypes.series,
      resolve: resolvers.series.one,
    },

    creator: {
      type: agentsTypes.agent,
      resolve: resolvers.agents.one,
    },

    channel: {
      type: channelsTypes.channel,
      resolve: resolvers.channels.one,
    },

    recentMeasurement: {
      description: 'Returns either objective measurement for a judged'
        + ' measurable or latest measurement of an agent.',
      type: require('./measurements').measurement,
      resolve: resolvers.measurements.latest,
    },

    outcome: {
      description: 'Returns latest objective measurement.',
      type: require('./measurements').measurement,
      resolve: resolvers.measurements.outcomeByRootId,
    },

    previousAggregate: {
      deprecated: 'Will be renamed on latestAggregate.',
      description: 'Returns latest aggregation measurement.',
      type: require('./measurements').measurement,
      resolve: resolvers.measurements.latestAggregateByRootId,
    },
  }),
});

const measurableCreateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurableCreateInput',
  fields: () => ({
    name,
    labelCustom,
    labelSubject,
    labelOnDate,
    labelProperty,
    valueType: { type: measurableValueType },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    expectedResolutionDate: { type: DateType.default },
    resolutionEndpoint: { type: graphql.GraphQLString },
    min: { type: graphql.GraphQLFloat },
    max: { type: graphql.GraphQLFloat },
  }),
});

const measurableUpdateInput = new graphql.GraphQLInputObjectType({
  name: 'MeasurableUpdateInput',
  fields: () => ({
    name,
    labelCustom,
    labelSubject,
    labelOnDate,
    labelProperty,
    valueType: { type: measurableValueType },
    channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    expectedResolutionDate: { type: DateType.default },
    resolutionEndpoint: { type: graphql.GraphQLString },
    min: { type: graphql.GraphQLFloat },
    max: { type: graphql.GraphQLFloat },
  }),
});

const measurablesEdge = new graphql.GraphQLObjectType({
  name: 'MeasurablesEdge',
  fields: () => ({
    node: { type: measurable },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const measurablesConnection = new graphql.GraphQLObjectType({
  name: 'MeasurablesConnection',
  fields: () => ({
    total: {
      type: graphql.GraphQLInt,
    },
    pageInfo: {
      type: graphql.GraphQLNonNull(commonTypes.pageInfoConnection),
    },
    edges: {
      type: graphql.GraphQLList(measurablesEdge),
    },
  }),
});

module.exports = {
  measurableState,
  measurable,
  measurableCreateInput,
  measurableUpdateInput,
  measurablesEdge,
  measurablesConnection,
};
