const _ = require('lodash');

const data = require('../../data');

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.auth0jwt
 * @param {Models.ObjectID} args.auth0accessToken
 * @param {Models.ObjectID} args.authToken
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<{jwt: string}>}
 */
async function exchangeToken(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');
  const auth0accessToken = _.get(args, 'auth0accessToken');

  if (!!auth0jwt && !!auth0accessToken) {
    return _exchangeAuthComToken(root, args, context, info);
  }

  return _exchangeAuthToken(root, args, context, info);
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.auth0jwt
 * @param {Models.ObjectID} args.auth0accessToken
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<{jwt: string}>}
 */
async function _exchangeAuthComToken(root, args, context, info) {
  const auth0jwt = _.get(args, 'auth0jwt');
  const auth0accessToken = _.get(args, 'auth0accessToken');

  const jwt = await data.authentication.exchangeAuthComToken(
    auth0jwt,
    auth0accessToken,
  );

  return { jwt };
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.authToken
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<{jwt: string}>}
 */
async function _exchangeAuthToken(root, args, context, info) {
  const authToken = _.get(args, 'authToken');
  const jwt = await data.authentication.exchangeAuthToken(authToken);
  return { jwt };
}

/**
 * @param {object | null} root
 * @param {object} args
 * @param {Models.ObjectID} args.id
 * @param {Schema.Context} context
 * @param {object} info
 * @returns {Promise<*>}
 */
async function getTokenByAgentId(root, args, context, info) {
  const agentId = _.get(root, 'agentId');
  return data.tokens.getCreate(agentId);
}

/**
 * @param {*} root
 * @param {object} args
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<{user: Models.User, bot: Models.Bot, agent: Models.Agent}>}
 */
async function authenticated(root, args, context, _info) {
  const user = _.get(context, 'user', null);
  const bot = _.get(context, 'bot', null);
  const agent = _.get(context, 'agent', null);
  return { user, bot, agent };
}

module.exports = {
  authenticated,
  exchangeToken,
  getTokenByAgentId,
};