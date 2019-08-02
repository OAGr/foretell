const { Producer } = require('../producer');
const { ProducerNotifications } = require('./producer-notifications');

class MemberInvitedToCommunity extends ProducerNotifications {
  constructor(input = {}) {
    super({});

    this.input = input;
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_INVITED_TO_COMMUNITY;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async main() {
    try {
      return true;
    } catch (e) {
      console.log(`MemberInvitedToCommunity`, e.message, e);
      return false;
    }
  }

}

module.exports = {
  MemberInvitedToCommunity,
};