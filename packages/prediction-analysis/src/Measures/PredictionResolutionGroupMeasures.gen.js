/* Untyped file generated by genType. */

const $$toRE824073253 = {"MarketScore": -870794282, "NonMarketScore": 463448073};

const Curry = require('bs-platform/lib/js/curry.js');

const PredictionResolutionGroupMeasuresBS = require('./PredictionResolutionGroupMeasures.bs');

const pointScore = function (Arg1, Arg21) {
  const result = Curry._5(PredictionResolutionGroupMeasuresBS.pointScore, (Arg1.marketType == null ? undefined : $$toRE824073253[Arg1.marketType]), Arg1.sampleCount, Arg1.scoringCombination, (Arg1.scoreFn == null ? undefined : function (ArgscoringCombination, ArgsampleCount, Arg2) {
      const result1 = Arg1.scoreFn({scoringCombination:ArgscoringCombination[0]===/* MarketScore */-870794282
        ? {tag:"MarketScore", value:ArgscoringCombination[1]}
        : {tag:"NonMarketScore", value:ArgscoringCombination[1]}, sampleCount:ArgsampleCount}, Arg2);
      return result1
    }), Arg21);
  return result
};;
exports.pointScore = pointScore