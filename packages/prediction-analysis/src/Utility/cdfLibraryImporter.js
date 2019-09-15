var {Pdf, Cdf, ContinuousDistribution, ContinuousDistributionCombination, scoringFunctions} = require("@foretold/cdf");

function cdfToPdf({xs, ys}){
    let cdf = new Cdf(xs, ys);
    let pdf = cdf.toPdf();
    return {xs: pdf.xs, ys: pdf.ys}
}

function mean(sampleCount, vars){
    let cdfs = vars.map(r => new Cdf(r.xs, r.ys));
    let comb = new ContinuousDistributionCombination(cdfs);
    let newCdf = comb.combineYsWithMean(sampleCount);
    return {xs: newCdf.xs, ys: newCdf.ys}
}

function scoreMarketCdfCdf(sampleCount, predictionCdf, aggregateCdf, resolutionCdf){
    let toCdf = (r) => (new Cdf(r.xs, r.ys));
    return scoringFunctions.distributionInputDistributionOutput({
        predictionCdf: toCdf(predictionCdf),
        aggregateCdf: toCdf(aggregateCdf),
        resultCdf: toCdf(resolutionCdf),
        sampleCount
    });
}

function scoreNonMarketCdfCdf(sampleCount, predictionCdf, resolutionCdf){
    let toCdf = (r) => (new Cdf(r.xs, r.ys));
    return scoringFunctions.distributionInputDistributionOutputMarketless({
        predictionCdf: toCdf(predictionCdf),
        resultCdf: toCdf(resolutionCdf),
        sampleCount
    });
}

function differentialEntropy(sampleCount, cdf){
    let toCdf = (r) => (new Cdf(r.xs, r.ys));
    return scoringFunctions.differentialEntropy({
        cdf: toCdf(cdf),
        sampleCount: sampleCount
    });
}

function findY(x, {xs, ys}){
    let cdf = new Cdf(xs, ys);
    let result = cdf.findY(x);
    return result
}

function findX(y, {xs, ys}){
    let cdf = new Cdf(xs, ys);
    let result = cdf.findX(y);
    return result
}

function integral({xs, ys}){
    let distribution = new ContinuousDistribution(xs, ys);
    return distribution.integral();
}

module.exports = {
    cdfToPdf,
    findY,
    findX,
    mean,
    scoreMarketCdfCdf,
    scoreNonMarketCdfCdf,
    differentialEntropy,
    integral
};