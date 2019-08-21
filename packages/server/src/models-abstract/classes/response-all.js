const utils = require('../../lib/utils');

class ResponseAll {
  /**
   * @public
   * @param {*[]} data
   * @param {number} total
   */
  constructor (data, total) {
    this.data = data;
    this.total = total;
  }

  /**
   * @public
   * @return {*[]}
   */
  getData() {
    return this.data;
  }

  /**
   * @public
   * @return {number}
   */
  getTotal() {
    return this.total;
  }

  inspect() {
    utils.inspect(this);
  }
}

module.exports = {
  ResponseAll,
};
