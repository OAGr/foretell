const graphql = require("graphql");
const { DateType } = require('graphql-sequelize');

const agentMeasurable = new graphql.GraphQLObjectType({
  name: 'AgentMeasurable',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    agentId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    measurableId: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
    primaryPointScore: { type: graphql.GraphQLNonNull(graphql.GraphQLFloat) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },

    // OK
    agent: {
      type: require('./agents').agent,
      resolve: require('../resolvers').agents.one,
    },

    // OK
    measurable: {
      type: require('./measurables').measurable,
      resolve: require('../resolvers').measurables.one,
    },

    // OK
    measurements: {
      type: require('./measurements').agentMeasurementsConnection,
      args: require('./common').connectionArguments,
      resolve: require('../resolvers').measurements.all,
    },
  })
});

const agentMeasurablesEdge = new graphql.GraphQLObjectType({
  name: 'AgentMeasurablesEdge',
  fields: () => ({
    node: { type: require('./agent-measurables').agentMeasurable },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const agentMeasurablesConnection = new graphql.GraphQLObjectType({
  name: 'AgentMeasurablesConnection',
  fields: () => ({
    total: { type: graphql.GraphQLInt },
    pageInfo: { type: graphql.GraphQLNonNull(require('./common').pageInfoConnection) },
    edges: { type: graphql.GraphQLList(require('./agent-measurables').agentMeasurablesEdge) },
  }),
});

module.exports = {
  agentMeasurable,
  agentMeasurablesEdge,
  agentMeasurablesConnection,
};
