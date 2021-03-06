const { DataBase } = require('./data-base');
const { NotificationModel } = require('./models');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {NotificationModel} model
 */
class NotificationsData extends DataBase {
  constructor() {
    super();
    this.model = new NotificationModel();
  }
}

module.exports = {
  NotificationsData,
};
