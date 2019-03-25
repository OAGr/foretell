const _ = require('lodash');

const graphql = require("graphql");
const { attributeFields, resolver } = require("graphql-sequelize");

const models = require("./models");
const data = require('./data');
const resolvers = require('./resolvers');

const types = require('./types');
const { stats } = require('./types/stats');
const { filterr } = require('./types/filterr');

const schema = new graphql.GraphQLSchema({
  query: new graphql.GraphQLObjectType({
    name: 'Query',
    fields: {

      user: {
        type: types.userType,
        args: {
          id: { type: graphql.GraphQLString },
          auth0Id: { type: graphql.GraphQLString },
        },
        resolve: data.usersData.getUser,
      },

      users: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.userType)),
        resolve: resolver(models.User),
      },

      // @todo: + authorizer
      measurement: {
        type: types.measurementType,
        args: _.pick(attributeFields(models.Measurement), ['id']),
        resolve: resolvers.measurements.one,
      },

      // @todo: + authorizer
      measurements: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.measurementType)),
        resolve: resolvers.measurements.all,
      },

      // @ok
      measurable: {
        type: types.measurableType,
        args: _.pick(attributeFields(models.Measurable), ['id']),
        resolve: resolvers.measurables.one,
      },

      // @ok
      measurables: {
        type: new graphql.GraphQLNonNull(new graphql.GraphQLList(types.measurableType)),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
          creatorId: { type: graphql.GraphQLString },
          seriesId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.measurables.all,
      },

      bot: {
        type: types.botType,
        args: _.pick(attributeFields(models.Bot), ['id']),
        resolve: resolver(models.Bot),
      },

      bots: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.botType)),
        resolve: resolver(models.Bot),
      },

      agent: {
        type: types.agentType,
        args: _.pick(attributeFields(models.Agent), ['id']),
        resolve: resolver(models.Agent),
      },

      agents: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.agentType)),
        resolve: resolver(models.Agent),
      },

      // @ok
      series: {
        type: types.seriesType,
        args: {
          id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.series.one,
      },

      // @ok
      seriesCollection: {
        type: new graphql.GraphQLNonNull(graphql.GraphQLList(types.seriesType)),
        args: {
          channelId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
        },
        resolve: resolvers.series.all,
      },

      // @ok, but null returns
      channel: {
        type: types.channels.channel,
        args: { id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) } },
        resolve: resolvers.channels.one,
      },

      // @ok
      channels: {
        type: graphql.GraphQLNonNull(graphql.GraphQLList(types.channels.channel)),
        args: {
          offset: { type: graphql.GraphQLInt },
          limit: { type: graphql.GraphQLInt },
        },
        resolve: resolvers.channels.all,
      },

      stats: {
        type: new graphql.GraphQLNonNull(stats),
        resolve: async (ops, values, options) => {
          // @todo:
          return 11;
        }
      },
    }
  }),
  mutation: new graphql.GraphQLObjectType({
    name: 'Mutation',
    fields: {

      // @ok
      createMeasurement: {
        type: types.measurementType,
        args: filterr(_.pick(attributeFields(models.Measurement), [
          'value', 'competitorType', 'measurableId', 'agentId', 'description'
        ])),
        resolve: resolvers.measurements.create,
      },

      createMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), [
          'name', 'description', 'valueType', 'expectedResolutionDate',
          'resolutionEndpoint', 'descriptionEntity', 'descriptionDate',
          'descriptionProperty', 'channelId',
        ])),
        resolve: resolvers.measurables.create,
      },

      createSeries: {
        type: types.seriesType,
        args: filterr(_.pick(attributeFields(models.Series), [
          'name', 'description', 'channelId', 'subjects', 'properties',
          'dates',
        ])),
        resolve: resolvers.series.create,
      },

      archiveMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: resolvers.measurables.archive,
      },

      unArchiveMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), ['id'])),
        resolve: resolvers.measurables.unarchive,
      },

      editMeasurable: {
        type: types.measurableType,
        args: filterr(_.pick(attributeFields(models.Measurable), [
          'id', 'name', 'description', 'expectedResolutionDate',
          'resolutionEndpoint', 'descriptionEntity', 'descriptionDate',
          'descriptionProperty',
        ])),
        resolve: resolvers.measurables.edit,
      },

      editUser: {
        type: types.userType,
        args: filterr(_.pick(attributeFields(models.User), ["id", "name"])),
        resolve: resolvers.users.edit,
      },

      channelUpdate: {
        type: types.channels.channel,
        args: {
          id: { type: graphql.GraphQLString },
          input: { type: new graphql.GraphQLNonNull(types.channels.channelInput) },
        },
        resolve: resolvers.channels.update,
      },

      channelCreate: {
        type: types.channels.channel,
        args: {
          input: { type: new graphql.GraphQLNonNull(types.channels.channelInput) },
        },
        resolve: resolvers.channels.create,
      },

      agentsChannelsCreate: {
        type: types.agentsChannels.agentsChannel,
        args: {
          agentId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.agentsChannels.create,
      },

      agentsChannelsDelete: {
        type: types.agentsChannels.agentsChannel,
        args: {
          agentId: { type: graphql.GraphQLString },
          channelId: { type: graphql.GraphQLString },
        },
        resolve: resolvers.agentsChannels.remove,
      },

    }
  })
});

module.exports = {
  schema,
};
