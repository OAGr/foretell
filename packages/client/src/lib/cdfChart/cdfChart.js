// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var CdfChartJs = require("./cdfChart.js");

var cdfChart = CdfChartJs.default;

function make(width, height, verticalLine, showVerticalLine, marginBottom, showDistributionLines, maxX, minX, primaryDistribution, children) {
  var tmp = { };
  if (width !== undefined) {
    tmp.width = Caml_option.valFromOption(width);
  }
  if (height !== undefined) {
    tmp.height = Caml_option.valFromOption(height);
  }
  if (verticalLine !== undefined) {
    tmp.verticalLine = Caml_option.valFromOption(verticalLine);
  }
  if (showVerticalLine !== undefined) {
    tmp.showVerticalLine = Caml_option.valFromOption(showVerticalLine);
  }
  if (showDistributionLines !== undefined) {
    tmp.showDistributionLines = Caml_option.valFromOption(showDistributionLines);
  }
  if (marginBottom !== undefined) {
    tmp.marginBottom = Caml_option.valFromOption(marginBottom);
  }
  if (maxX !== undefined) {
    tmp.maxX = Caml_option.valFromOption(maxX);
  }
  if (minX !== undefined) {
    tmp.minX = Caml_option.valFromOption(minX);
  }
  if (primaryDistribution !== undefined) {
    tmp.primaryDistribution = Caml_option.valFromOption(primaryDistribution);
  }
  return ReasonReact.wrapJsForReason(cdfChart, tmp, children);
}

exports.cdfChart = cdfChart;
exports.make = make;
/* cdfChart Not a pure module */
