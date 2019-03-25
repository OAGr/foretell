const _ = require('lodash');

const data = require('../data');

async function channel(root, args, context, info) {
  const channelId = _.get(args, 'channelId')
    || _.get(root, 'channelId')
    || _.get(context, 'channelId');
  console.log('\x1b[36m ---> \x1b[0m Middleware (channel)', { channelId });
  context.channel = channelId
    ? await data.channelsData.getOne(channelId)
    : null;
}

module.exports = {
  channel,
};
