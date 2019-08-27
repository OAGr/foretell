module.exports = (sequelize, DataTypes) => {
  const AgentMeasurable = sequelize.define('AgentMeasurable', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    agentId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    measurableId: {
      type: DataTypes.STRING,
      allowNull: false,
    },
    primaryPointScore: {
      type: DataTypes.FLOAT,
      allowNull: false,
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: DataTypes.NOW,
    },
  });

  AgentMeasurable.associate = function associate(models) {
    AgentMeasurable.Agent = AgentMeasurable.belongsTo(models.Agent, {
      foreignKey: 'agentId',
    });
    AgentMeasurable.Measurable = AgentMeasurable.belongsTo(models.Measurable, {
      foreignKey: 'measurableId',
    });
  };

  return AgentMeasurable;
};