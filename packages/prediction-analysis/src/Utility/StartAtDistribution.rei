type t('a, 'b) = {
  finalX: 'a,
  pointXY: array(('a, 'b)),
};

let make: (~pointXY: array(('a, 'b)), ~finalX: 'a) => t('a, 'b);

// Finds relevant (x,y) point at or before a specific x point.
let xPointToRelevantPointXY: ('a, t('a, 'b)) => option(('a, 'b));

let product: (t('a, 'b), t('a, 'c)) => t('a, ('b, 'c));

let map: ('b => 'c, t('a, 'b)) => t('a, 'c);

let transposeResult:
  t('a, Belt.Result.t('b, 'c)) => Belt.Result.t(t('a, 'b), 'c);

let integral: t(float, float) => Belt.Result.t(float, string);

let average:
  (~t: t(float, float), ~lowestTime: float=?, ~highestTime: float=?, unit) =>
  Belt.Result.t(float, string);