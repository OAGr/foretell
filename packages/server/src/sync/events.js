module.exports = {
  // Server is ready
  SERVER_IS_READY: 'SERVER_IS_READY',

  // Sync Signals
  EVERY_HOUR: 'EVERY_HOUR',
  EVERY_TEN_MINUTES: 'EVERY_TEN_MINUTES',
  EVERY_THREE_MINUTES: 'EVERY_THREE_MINUTES',
  EVERY_MINUTE: 'EVERY_MINUTE',

  // Encapsulated Events Flow
  MAIL: 'MAIL',

  // System Events (after)
  NEW_MEASUREMENT: 'NEW_MEASUREMENT',
  NEW_MEASURABLE: 'NEW_MEASURABLE',
  NEW_MEMBERSHIP: 'NEW_MEMBERSHIP',
  NEW_CHANNEL: 'NEW_CHANNEL',
  NEW_SERIES: 'NEW_SERIES',
  NEW_INVITATION: 'NEW_INVITATION',
  NEW_USER: 'NEW_USER',
  NEW_NOTEBOOK: 'NEW_NOTEBOOK',

  // (after)
  MEASURABLE_CHANGED: 'MEASURABLE_CHANGED',
  USER_CHANGED: 'USER_CHANGED',
  SERIES_CHANGED: 'SERIES_CHANGED',

  //
  CREATING_BOT: 'CREATING_BOT',
  CREATING_USER: 'CREATING_USER',
  UPDATING_MEASURABLE: 'UPDATING_MEASURABLE',
  VALIDATING_MEASUREMENT: 'VALIDATING_MEASUREMENT',
};
