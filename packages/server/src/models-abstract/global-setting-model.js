const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.AbstractModelsLayer.AbstractModel}
 */
class GlobalSettingModel extends ModelPostgres {

  constructor() {
    super({
      model: models.GlobalSetting,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  GlobalSettingModel,
};