const _ = require('lodash');
const assert = require('assert');
const { Op } = require('sequelize');

const models = require('../models');
const logger = require('../lib/log');

const { Model } = require('./model');
const { ResponseAll } = require('./classes');


/**
 * @abstract
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class ModelPostgres extends Model {
  constructor(
    {
      model,
      sequelize,
    },
  ) {
    super();

    this.model = model;
    this.models = models;
    this.sequelize = sequelize;

    this.Op = Op;
    this.in = Op.in;
    this.gt = Op.gt;
    this.gte = Op.gte;
    this.lt = Op.lt;
    this.lte = Op.lte;
    this.and = Op.and;
    this.not = Op.not;
    this.notIn = Op.notIn;
    this.or = Op.or;

    this.fn = this.sequelize.fn;
    this.col = this.sequelize.col;
    this.literal = this.sequelize.literal;

    this.log = logger.module('models-abstract/model-postgres');
  }

  /**
   * @protected
   * @todo: see this._publicAndJoinedChannels()
   * @param {Models.AgentID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _publicAndJoinedChannelsLiteral(agentId, name = '') {
    return this.literal(this._publicAndJoinedChannels(agentId, name));
  }

  /**
   * @protected
   * @param {Models.AgentID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _publicAndJoinedChannels(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* P͟u͟b͟l͟i͟c͟ ͟a͟n͟d͟ ͟J͟o͟i͟n͟e͟d͟ ͟C͟h͟a͟n͟n͟e͟l͟s͟ (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN
        "ChannelMemberships"
        ON "Channels".id = "ChannelMemberships"."channelId"
        AND "ChannelMemberships"."agentId" = '${agentId}'
      WHERE
        "ChannelMemberships"."agentId" IS NOT NULL
        OR "Channels"."isPublic" = TRUE
    )`;
  }

  /**
   * @protected
   * @para {string} [name]
   * @return {Sequelize.literal}
   */
  _publicChannelsLiteral(name = '') {
    return this.literal(this._publicChannels(name));
  }

  /**
   * @protected
   * @param {string} [name]
   * @return {string}
   */
  _publicChannels(name = '') {
    return `(
      /* P͟u͟b͟l͟i͟c͟ ͟C͟h͟a͟n͟n͟e͟l͟s͟ (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      WHERE "Channels"."isPublic" = TRUE
    )`;
  }

  /**
   * @todo: Rename to "withinJoinedChannels" + "Literal"?
   * @protected
   * @param {Models.AgentID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _joinedChannelsLiteral(agentId, name = '') {
    return this.literal(this._joinedChannels(agentId, name));
  }

  /**
   * @todo: Rename to "withinJoinedChannels"?
   * @protected
   * @param {Models.AgentID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _joinedChannels(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* J͟o͟i͟n͟e͟d͟ ͟C͟h͟a͟n͟n͟e͟l͟s͟ (${name}) */
      SELECT "Channels"."id" FROM "Channels"
      LEFT OUTER JOIN
        "ChannelMemberships"
        ON "Channels".id = "ChannelMemberships"."channelId"
        AND "ChannelMemberships"."agentId" = '${agentId}'
      WHERE
        "ChannelMemberships"."agentId" IS NOT NULL
    )`;
  }

  /**
   * @protected
   * @param {Models.AgentID} [agentId]
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _measurablesInPublicAndJoinedChannelsLiteral(agentId, name = '') {
    return this.literal(
      this._measurablesInPublicAndJoinedChannels(agentId, name),
    );
  }

  /**
   * @protected
   * @param {Models.AgentID} [agentId]
   * @param {string} [name]
   * @return {string}
   */
  _measurablesInPublicAndJoinedChannels(agentId, name = '') {
    assert(!!agentId, 'Agent ID is required.');
    return `(
      /* Measurables in Public and Joined Channels (${name}) */
      WITH channelIds AS (${this._publicAndJoinedChannels(agentId, name)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @protected
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _measurablesInPublicChannelsLiteral(name = '') {
    return this.literal(
      this._measurablesInPublicChannels(name),
    );
  }

  /**
   * @protected
   * @param {string} [name]
   * @return {string}
   */
  _measurablesInPublicChannels(name = '') {
    return `(
      /* Measurables in Public Channels (${name}) */
      WITH channelIds AS (${this._publicChannels(name)})
      SELECT "Measurables"."id" FROM "Measurables"
      WHERE "Measurables"."channelId" IN (SELECT id FROM channelIds)
    )`;
  }

  /**
   * @protected
   * @param {string[]} statesIn
   * @param {Models.ChannelID} channelIdIn
   * @param {string} [name]
   * @return {Sequelize.literal}
   */
  _withinMeasurablesLiteral(statesIn, channelIdIn, name = '') {
    return this.literal(
      this._withinMeasurables(statesIn, channelIdIn, name),
    );
  }

  /**
   * @protected
   * @param {string[] | null} statesIn
   * @param {Models.ChannelID | null} channelIdIn
   * @param {string} [name]
   * @return {string}
   */
  _withinMeasurables(statesIn, channelIdIn, name = '') {
    const cond = [];
    const states = _.isArray(statesIn)
      ? statesIn.map((state) => `'${state}'`)
        .join(', ') : [];

    if (states.length > 0) cond.push(`("state" IN (${states}))`);
    if (!!channelIdIn) cond.push(`("channelId" = '${channelIdIn}')`);

    const where = cond.length > 0 ? `WHERE (${cond.join(' AND ')})` : '';

    return `(
      /* Within Measurables (${name}) */
      SELECT "id" FROM "Measurables"
      ${where}
    )`;
  }

  /**
   * @todo: To add mind map from Params to DB query.
   * @protected
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {object}
   */
  applyRestrictions(where = {}, restrictions = {}) {
    if (restrictions.isAdmin) return where;
    if (!where[this.and]) where[this.and] = [];

    this.applyAbstracts(where, restrictions);

    // @todo: Use "withinPublicChannels" as "channelId"
    if (!!restrictions.channelId && !restrictions.agentId) {
      where[this.and].push({
        channelId: {
          [this.in]: this._publicChannelsLiteral('Restrictions'),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "channelId"
    if (!!restrictions.channelId && !!restrictions.agentId) {
      where[this.and].push({
        channelId: {
          [this.in]: this._publicAndJoinedChannelsLiteral(
            restrictions.agentId,
            'Restrictions',
          ),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "id"
    if (!!restrictions.channelIdAsId && !!restrictions.agentId) {
      where[this.and].push({
        id: {
          [this.in]: this._publicAndJoinedChannelsLiteral(
            restrictions.agentId,
            'Restrictions',
          ),
        },
      });
    }

    // @todo: Use "withinPublicAndJoinedChannels" as "id"
    if (!!restrictions.channelIdAsId && !restrictions.agentId) {
      where[this.and].push({
        id: {
          [this.in]: this._publicChannelsLiteral('Restrictions'),
        },
      });
    }

    if (!!restrictions.userId) {
      where[this.and].push({
        userId: restrictions.userId,
      });
    }

    // @todo: Use structures.
    if (!!restrictions.measurableId && !!restrictions.agentId) {
      where[this.and].push({
        measurableId: {
          [this.in]: this._measurablesInPublicAndJoinedChannelsLiteral(
            restrictions.agentId,
            'Restrictions',
          ),
        },
      });
    }

    // @todo: Use structures.
    if (!!restrictions.measurableId && !restrictions.agentId) {
      where[this.and].push({
        measurableId: {
          [this.in]: this._measurablesInPublicChannelsLiteral(
            'Restrictions',
          ),
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * @todo: Try to fix it and do not use "this.models.Measurement".
   * @param {object} [include]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @return {*}
   */
  applyRestrictionsIncluding(include = [], restrictions = {}) {
    if (!include) include = [];

    // @todo: It is a filter, b͟u͟t͟ ͟n͟o͟t͟ ͟r͟e͟s͟t͟r͟i͟c͟t͟i͟o͟n͟
    // @todo: Use object structures.
    if (!!restrictions.measuredByAgentId) {
      include.push({
        model: this.models.Measurement,
        as: 'Measurements',
        where: { agentId: restrictions.measuredByAgentId },
      });
    }

    return include;
  }

  /**
   * If you define filters in the generic type Filter
   * then write code only in this parent method.
   * Do not place pieces of code in child classes.
   * Since all child classes should have access to these
   * filters.
   *
   * @protected
   * Extend this method in child classes.
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Models.AgentID} [filter.userId]
   */
  applyFilter(where = {}, filter = {}) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

    this.applyAbstracts(where, filter);

    if (filter.isArchived) {
      where[this.and].push({
        isArchived: {
          [this.in]: this._getBooleansOfList(filter.isArchived),
        },
      });
    }

    if (!!filter.measurableIds) {
      where[this.and].push({
        id: {
          [this.in]: filter.measurableIds,
        },
      });
    }

    if (!!filter.userId) {
      where[this.and].push({
        userId: filter.userId,
      });
    }

    if (!!filter.agentId) {
      where[this.and].push({
        agentId: filter.agentId,
      });
    }

    if (!!filter.channelId) {
      where[this.and].push({
        channelId: filter.channelId,
      });
    }

    if (!!filter.measurableId) {
      where[this.and].push({
        measurableId: filter.measurableId,
      });
    }

    if (!!filter.email) {
      where[this.and].push({
        email: filter.email,
      });
    }

    if (!!filter.status) {
      where[this.and].push({
        status: filter.status,
      });
    }

    if (!!filter.competitorType) {
      where[this.and].push({
        competitorType: {
          [this.in]: filter.competitorType,
        },
      });
    }

    if (!!filter.notTaggedByAgent) {
      where[this.and].push({
        id: {
          [this.notIn]: this._taggedMeasurementsLiteral(
            filter.notTaggedByAgent,
          ),
        },
      });
    }

    const startDate = _.get(filter, 'findInDateRange.startDate');
    if (!!startDate) {
      where[this.and].push({
        createdAt: { [this.gte]: startDate },
      });
    }

    const endDate = _.get(filter, 'findInDateRange.endDate');
    if (!!endDate) {
      where[this.and].push({
        createdAt: { [this.lte]: endDate },
      });
    }

    if (_.isArray(filter.states)) {
      where[this.and].push({
        state: { [this.in]: filter.states },
      });
    }

    if (!!filter.seriesId) {
      where[this.and].push({
        seriesId: filter.seriesId,
      });
    }

    if (!!filter.ownerId) {
      where[this.and].push({
        ownerId: filter.ownerId,
      });
    }

    if (!!filter.creatorId) {
      where[this.and].push({
        creatorId: filter.creatorId,
      });
    }

    if (!!filter.isEmailVerified) {
      where[this.and].push({
        isEmailVerified: {
          [this.in]: filter.isEmailVerified,
        },
      });
    }

    if (filter.isNotEmailVerified) {
      where[this.and].push({
        [this.or]: [
          { isEmailVerified: false },
          { isEmailVerified: null },
        ],
      });
    }

    if (filter.notAuth0AccessToken) {
      where[this.and].push({
        auth0AccessToken: {
          [this.not]: null,
        },
      });
    }

    if (!!filter.excludeChannelId) {
      where[this.and].push({
        id: {
          [this.notIn]: this._agentsIdsLiteral(filter.excludeChannelId),
        },
      });
    }

    if (!!filter.types) {
      where[this.and].push({
        type: {
          [this.in]: filter.types,
        },
      });
    }

    if (filter.sentAt || filter.sentAt === null) {
      where[this.and].push({
        sentAt: filter.sentAt,
      });
    }

    if (!!filter.notificationId) {
      where[this.and].push({
        notificationId: filter.notificationId,
      });
    }

    if (filter.attemptCounterMax) {
      where[this.and].push({
        attemptCounter: {
          [this.lt]: filter.attemptCounterMax,
        },
      });
    }

    if (_.isNumber(filter.minPredictionCountTotal)) {
      where[this.and].push({
        predictionCountTotal: {
          [this.gte]: filter.minPredictionCountTotal,
        },
      });
    }

    if (_.isNumber(filter.minNumberOfPredictions)) {
      where[this.and].push({
        numberOfPredictions: {
          [this.gte]: filter.minNumberOfPredictions,
        },
      });
    }

    if (_.isNumber(filter.minNumberOfQuestionsScored)) {
      where[this.and].push({
        numberOfQuestionsScored: {
          [this.gte]: filter.minNumberOfQuestionsScored,
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * Extend this method in child classes.
   * @param {object} [where]
   * @param {Layers.AbstractModelsLayer.filter
   * | Layers.AbstractModelsLayer.restrictions} [abstractions]
   */
  applyAbstracts(where = {}, abstractions = {}) {
    if (!where) where = {};
    if (!where[this.and]) where[this.and] = [];

    const name = _.get(abstractions, 'constructor.name', 'Abstraction');

    if (!!abstractions.withinPublicChannels) {
      const { as } = abstractions.withinPublicChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._publicChannelsLiteral(name),
        },
      });
    }

    if (!!abstractions.withinJoinedChannels) {
      const { as, agentId } = abstractions.withinJoinedChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._joinedChannelsLiteral(agentId, name),
        },
      });
    }

    if (!!abstractions.withinPublicAndJoinedChannels) {
      const { as, agentId } = abstractions.withinPublicAndJoinedChannels;
      where[this.and].push({
        [as]: {
          [this.in]: this._publicAndJoinedChannelsLiteral(agentId, name),
        },
      });
    }

    if (!!abstractions.withinMeasurables) {
      const { as, states, channelId } = abstractions.withinMeasurables;
      where[this.and].push({
        [as]: {
          [this.in]: this._withinMeasurablesLiteral(states, channelId, name),
        },
      });
    }

    return where;
  }

  /**
   * @protected
   * @param {*[]} [list]
   * @return {*[]}
   */
  _getBooleansOfList(list = []) {
    return list.map((item) => {
      if (item === 'TRUE') {
        return true;
      }
      if (item === 'FALSE') {
        return false;
      }
      return item;
    });
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.data} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise.<object>}
   */
  async createOne(data = {}, _restrictions = {}, options = {}) {
    return this.model.create(data, options);
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.data} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise.<object>}
   */
  async updateOne(params = {}, data = {}, _restrictions = {}, options = {}) {
    const findCond = { where: { ...params } };
    const updateCond = {};
    this._extendConditions(findCond, options);
    this._extendConditions(updateCond, options);

    const entity = await this.model.findOne(findCond);
    if (!!entity) await entity.update(data, updateCond);
    return entity;
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.data} [data]
   * @param {Layers.AbstractModelsLayer.restrictions} [_restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {boolean}
   */
  async updateAll(params = {}, data = {}, _restrictions = {}, options = {}) {
    const cond = { where: { ...params } };
    this._extendConditions(cond, options);
    return !!(await this.model.update(data, cond));
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.filter} filter
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<*[]>}
   */
  async getAll(
    filter = {},
    pagination = {},
    restrictions = {},
    options = {},
  ) {
    const where = {};

    if ('inspect' in filter) filter.inspect();
    if ('inspect' in pagination) pagination.inspect();
    if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);
    this.applyFilter(where, filter);

    const cond = {
      limit: pagination.limit,
      offset: pagination.offset,
      where,
    };

    this._extendConditions(cond, options);

    return this.model.findAll(cond);
  }

  /**
   * @todo: This is an anisotropy when pagination is defined {}
   * @todo: instead of pagination = new Pagination().
   * @public
   * @param {Layers.AbstractModelsLayer.filter} [filter]
   * @param {Layers.AbstractModelsLayer.pagination} [pagination]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<{data: Models.Model[], total: number}>}
   */
  async getAllWithConnections(
    filter = {},
    pagination = {},
    restrictions = {},
    options = {},
  ) {
    const where = {};
    const include = [];

    if ('inspect' in filter) filter.inspect();
    if ('inspect' in pagination) pagination.inspect();
    if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);
    this.applyRestrictionsIncluding(include, restrictions);
    this.applyFilter(where, filter);

    const cond = { where, include };
    this._extendConditions(cond, options);

    /** @type {number} */
    const total = await this.model.count(cond);
    const { limit, offset } = pagination.getPagination2();

    const order = pagination.isOrderSet()
      ? this._getDefaultOrder(pagination)
      : this._getOrder();

    const findCond = {
      ...cond,
      limit,
      offset,
      order,
      attributes: this._getAttributes(),
    };

    /** @type {Models.Model[]} */
    const data = await this.model.findAll(findCond);

    return new ResponseAll(
      data,
      total,
      offset,
      filter.getSpacedLimit(),
    );
  }

  /**
   * @param pagination
   * @returns {[any, any][]}
   * @private
   */
  _getDefaultOrder(pagination) {
    return pagination.getOrder()
      .map((item) => ([item.field, item.direction]));
  }

  /**
   * @return {*[] | null}
   * @private
   */
  _getOrder() {
    return [['createdAt', 'DESC']];
  }

  /**
   *
   * @return {{include: Sequelize.literal|*[]} | null}
   * @private
   */
  _getAttributes() {
    return null;
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.query} [query]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<Models.Model>}
   */
  async getOne(params = {}, query = {}, restrictions = {}, options = {}) {
    const cond = await this.getPredicated(params, query, restrictions, options);
    return this.model.findOne(cond);
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.query} [query]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<Models.Model>}
   */
  async getCount(params = {}, query = {}, restrictions = {}, options = {}) {
    const cond = await this.getPredicated(params, query, restrictions, options);
    return this.model.count(cond);
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.query} [query]
   * @param {Layers.AbstractModelsLayer.restrictions} [restrictions]
   * @param {Layers.AbstractModelsLayer.options} [options]
   * @return {Promise<*>}
   */
  async getPredicated(
    params = {},
    query = {},
    restrictions = {},
    options = {},
  ) {
    const where = { ...params };
    const sort = query.sort === 1 ? 'ASC' : 'DESC';
    const order = [['createdAt', sort]];
    const distinct = !!query.distinct ? true : null;
    const col = !!query.col ? query.col : null;

    if ('inspect' in params) params.inspect();
    if ('inspect' in restrictions) restrictions.inspect();

    this.applyRestrictions(where, restrictions);

    const cond = {
      where,
      order,
      distinct,
      col,
    };

    this._extendConditions(cond, options);

    return cond;
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.query} [query]
   * @param {Layers.AbstractModelsLayer.data} data
   * @param {Layers.AbstractModelsLayer.restrictions} restrictions
   * @param {Layers.AbstractModelsLayer.options} options
   * @return {Promise<Models.Model>}
   */
  async upsertOne(params, query, data, restrictions, options) {
    return await this.getOne(params, query, restrictions, options)
      || await this.createOne(data, restrictions, options);
  }

  /**
   * @public
   * @param {Layers.AbstractModelsLayer.params} [params]
   * @param {Layers.AbstractModelsLayer.query} [query]
   * @param {Layers.AbstractModelsLayer.restrictions} restrictions
   * @param {Layers.AbstractModelsLayer.options} options
   * @return {Promise<Models.Model>}
   */
  async deleteOne(params, query, restrictions, options) {
    const entity = await this.getOne(params, query, restrictions, options);
    if (entity) {
      const where = { ...params };
      const cond = { where };
      this._extendConditions(cond, options);
      await this.model.destroy(cond);
    }
    return entity;
  }

  /**
   * @public
   * @return {Promise<*>}
   */
  async getTransaction() {
    return this.sequelize.transaction();
  }

  /**
   * @public
   * @param {object} transaction
   * @return {Promise<*>}
   */
  async commit(transaction) {
    return transaction.commit();
  }

  /**
   * @public
   * @param {object} transaction
   * @return {Promise<*>}
   */
  async rollback(transaction) {
    return transaction.rollback();
  }

  /**
   * @param {string} name
   * @param {Layers.AbstractModelsLayer.options} options
   * @returns {Promise<*>}
   */
  async _lockTable(name, options = {}) {
    const cond = {};
    this._extendConditions(cond, options);
    await this.sequelize.query(`SET LOCAL lock_timeout = '3s'`, cond);
    return this.sequelize.query(`LOCK TABLE "${name}"`, cond);
  }

  /**
   * @protected
   * @param {object} cond
   * @param {Layers.AbstractModelsLayer.options} options
   */
  _extendConditions(cond = {}, options = {}) {
    if (_.has(options, 'transaction')) {
      cond.transaction = options.transaction;
    }
    if (_.has(options, 'lock')) {
      cond.lock = options.lock;
    }
    if (_.has(options, 'skipLocked')) {
      cond.skipLocked = options.skipLocked;
    }
    return cond;
  }
}

module.exports = {
  ModelPostgres,
};
