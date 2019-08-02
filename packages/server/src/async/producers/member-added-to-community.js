const { Producer } = require('./producer');
const { MemberToCommunity } = require('./member-to-community');

class MemberAddedToCommunity extends MemberToCommunity {
  constructor(channelMembership = {}) {
    super(channelMembership);
    this.templateName = Producer.TEMPLATE_NAME.MEMBER_ADDED_TO_COMMUNITY;
  }
}

module.exports = {
  MemberAddedToCommunity,
};
