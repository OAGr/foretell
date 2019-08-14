const models = require('../models');
const { MeasurementsData } = require('./measurements-data');

describe('Measurement Data layer', () => {

  it('class should be a constructor', () => {
    expect(MeasurementsData).toBeInstanceOf(Function);
  });

  const instance = new MeasurementsData();

  describe('getOne()', () => {
    const id = 'id2';
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(models.Measurement, 'findOne').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds a measurement', () => {
      return instance.getOne2(id, options).then((result) => {
        expect(models.Measurement.findOne).toHaveBeenCalledWith({
          "where": {
            "id": "id2",
            "measurableId": {
              [instance.model.Op.in]: "measurableIdsLiteral"
            },
          }
        });
        expect(result).toBe(true);
      });
    });
  });

  describe('getAll()', () => {
    const options = { agentId: 'agentId1' };
    beforeEach(() => {
      jest.spyOn(instance.model, 'getAll').mockReturnValue(
        Promise.resolve(true),
      );
    });
    it('finds a measurement', () => {
      return instance.getAll({}, {}, options).then((result) => {
        expect(instance.model.getAll).toHaveBeenCalledWith(
          {},
          {},
          { "agentId": "agentId1", "measurableId": true },
        );
        expect(result).toBe(true);
      });
    });
  });


});
