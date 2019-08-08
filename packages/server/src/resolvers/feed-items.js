const _ = require('lodash');

const data = require('../data');
const { Pagination } = require('../data/classes/pagination');
const { Options } = require('../data/classes/options');
const { Filter } = require('../data/classes/filter');

/**
 * @param {*} root
 * @param {object} args
 * @param {Models.ObjectID} args.channelId
 * @param {Models.ObjectID} args.agentId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<Models.Model[]>}
 */
async function all(root, args, context, info) {
  const channelId = _.get(args, 'channelId');
  const agentId = _.get(args, 'agentId');
  const channelMemberId = _.get(context, 'agent.id');

  const filter = new Filter({ channelId, agentId });
  const pagination = new Pagination(args);
  const options = new Options({ channelMemberId });
  const connection = await data.feedItems.getConnection(
    filter,
    pagination,
    options,
  );

  return connection.getData();
}

module.exports = {
  all,
};
