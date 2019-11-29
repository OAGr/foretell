module.exports = (sequelize, DataTypes) => {
  const Notebook = sequelize.define('Notebook', {
    id: {
      type: DataTypes.UUID(),
      primaryKey: true,
      defaultValue: sequelize.fn('uuid_generate_v4'),
      allowNull: false,
    },
    name: {
      type: DataTypes.STRING(255),
      allowNull: false,
      validate: {
        len: [3, 255],
      },
    },
    channelId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    ownerId: {
      type: DataTypes.UUID(),
      allowNull: false,
    },
    body: {
      type: DataTypes.TEXT,
      allowNull: false,
      validate: {
        len: [3, 16*1024],
      },
    },
    createdAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
    updatedAt: {
      type: DataTypes.DATE,
      defaultValue: sequelize.fn('statement_timestamp'),
    },
  });

  Notebook.associate = function associate(models) {
    Notebook.Channel = Notebook.belongsTo(models.Channel, {
      foreignKey: 'channelId',
    });
    Notebook.Agent = Notebook.belongsTo(models.Agent, {
      foreignKey: 'ownerId',
    });
  };

  return Notebook;
};
