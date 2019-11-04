import _ from "lodash";
import math from 'mathjs';  

const SUFFIXES = {
  '%': -2,
  'K': 3,
  'M': 6,
  'B': 9,
  'T': 12,
}

const parenthesize = str => `(?:${str})`
//TODO
const escSpecialChars = str => str.replace(/\$|\{|\}|\_/g, e => `\\${e}`)
export const isPresent = e => (!!e && !_.isEmpty(e)) || (typeof e === 'number') || (e === true)
const toSource = re => re instanceof RegExp ? re.source : escSpecialChars(re)
function or(res) {
  const strParts = res.map(toSource).filter(isPresent).map(parenthesize)
  const lengthSorted = _.sortBy(strParts, prt => -prt.length) // To avoid partial replacements.
  return new RegExp(parenthesize(lengthSorted.join('|')), 'g')
}

const spaceSep = res => new RegExp(res.filter(re => !!re).map(re => `(?:${re.source})`).join('\\s*'))
const padded = res => spaceSep([/^/, ...res, /$/])

const SUFFIX_REGEX = new RegExp(Object.keys(SUFFIXES).join('|'))
const INTEGER_REGEX = /(?:(?:\d+)|(?:\d{1,3}(?:,\d{3})*))(?!\.[^\.])/
const DECIMAL_REGEX = /\d*\.\d+/
const NUMBER_REGEX = new RegExp(`(-?${or([INTEGER_REGEX, DECIMAL_REGEX]).source})\\s?(${SUFFIX_REGEX.source})?`)

export const POINT_REGEX = padded([NUMBER_REGEX])
export const rangeRegex = (sep, left, right) => padded([left, NUMBER_REGEX, sep, NUMBER_REGEX, right])

const getMult = suffix => Math.pow(10,SUFFIXES[suffix])
const parseNumber = (num, suffix) => parseFloat(num.replace(',', '')) * (!!suffix ? getMult(suffix) : 1)

const rangeErrorFn = ([low, high]) => low > high ? {type: 1, subType: 2} : {}

// We assume that if the user started at 0 or tried a negative number,
// they intended for this to be normal.
function getGuesstimateType(guesstimateType, [low]) {
  switch (guesstimateType) {
    case 'UNIFORM': return guesstimateType
    case 'NORMAL': return guesstimateType
    default: return low <= 0 ? 'NORMAL' : 'LOGNORMAL'
  }
}

export function regexBasedFormatter(re, guesstimateTypeFn = getGuesstimateType, errorFn = rangeErrorFn) {
  return {
    matches({text}) { return re.test(text) },
    error({text}) { return errorFn(this._numbers(text)) },

    format({guesstimateType, text}) {
      const params = this._numbers(text)
      return {guesstimateType: guesstimateTypeFn(guesstimateType, params), params}
    },

    _numbers(text) { return _.chunk(text.match(re).slice(1), 2).map(([num, suffix]) => parseNumber(num, suffix)) },
  }
}

export function NumberToNumberIntoLognormal(text){
    
    // This function is used in foretold/packages/guesstimator/formatter/formatters/Function.js
    // In order to be able to format inputs like "1 to 100" when inside a multimodal, like "=mm(normal(10,5), 1 to 100))".
    
    function replacerNumberToNumber(match, p1, p2){
    
        p1 = Number(p1)
        p2 = Number(p2)
        let logHigh = math.log(p1)
        let logLow = math.log(p2)

        let mean = math.mean(p1,p2)
        let stdev = (logHigh-logLow) / (2*1.645)
        
        return `lognormal(${mean}, ${stdev})`

    } // That this function is inside might be marginally slower, but it makes clear that replacerNumberToNumber is not used by any other function.

    let regNumberToNumber= new RegExp("([0-9]+) to ([0-9]+)", "g");
    
    return text.replace(regNumberToNumber, replacerNumberToNumber);
    // replace can take a function, rather than a string, as its second argument.
    // See: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String/replace#Specifying_a_function_as_a_parameter

}
