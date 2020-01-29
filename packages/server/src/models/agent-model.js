const assert = require('assert');

const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class AgentModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Agent,
      sequelize: models.sequelize,
    });
  }

  /**
   * @todo: see this._publicAndJoinedChannels()
   * @protected
   * @param {Models.ChannelID} channelId
   * @return {Sequelize.literal}
   */
  _agentsIdsLiteral(channelId) {
    return this.literal(this._agentsIds(channelId));
  }

  /**
   * @todo: Use ORM opportunities to join tables.
   * @todo: No, do not this ORM for this.
   * @protected
   * @param {Models.ChannelID} channelId
   * @return {string}
   */
  _agentsIds(channelId) {
    assert(!!channelId, 'Channel ID is required.');
    return `(
      SELECT "ChannelMemberships"."agentId" FROM "ChannelMemberships"
      WHERE "ChannelMemberships"."channelId" = '${channelId}'
    )`;
  }

  /**
   * @param _options
   * @returns {*}
   * @protected
   */
  _getAttributes(_options = {}) {
    return {
      include: [
        [this._nameLiteral(), 'name'],
      ],
    };
  }

  /**
   * @protected
   * @return {Sequelize.literal}
   */
  _nameLiteral() {
    return this.literal(this._name());
  }

  /**
   * @todo: To fix later.
   * @protected
   * @return {string}
   */
  _name() {
    // eslint-disable-next-line max-len
    return `( CASE WHEN (SELECT "name" FROM "Users" WHERE "agentId" = "Agent"."id" LIMIT 1)  <> ''
    THEN (SELECT "name" FROM "Users" WHERE "agentId" = "Agent"."id" LIMIT 1)
    ELSE (SELECT "name" FROM "Bots" WHERE "agentId" = "Agent"."id" LIMIT 1)
    END )`;
  }
}

module.exports = {
  AgentModel,
};
