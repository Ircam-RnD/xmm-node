/*
 * This library is developed by the ISMM (http://ismm.ircam.fr/) team at IRCAM,
 * within the context of the RAPID-MIX (http://rapidmix.goldsmithsdigital.com/)
 * project, funded by the European Union’s Horizon 2020 research and innovation programme.
 * Original XMM code authored by Jules Françoise, ported to Node.js by Joseph Larralde.
 * See https://github.com/Ircam-RnD/xmm for detailed XMM credits.
 */

/** @module xmm */

// var XmmNative = null;

//  NO NEED TO DO THIS ANYMORE THANKS TO NODE-PRE-GYP :

/** @todo pre-build binaries for windows */

// Load the precompiled binary for windows.

// if (process.platform == "win32" && process.arch == "x64") {
//  XmmNative = require('./bin/winx64/xmm');
// } else if(process.platform == "win32" && process.arch == "ia32") {
//  XmmNative = require('./bin/winx86/xmm');
// } else {

var binary = require('node-pre-gyp');
var path = require('path');
var bindingPath = binary.find(path.resolve(path.join(__dirname, './package.json')));

// Load the new built binary for other platforms.
// XmmNative = require('./build/Release/xmm-node');
var XmmNative = require(bindingPath);

// }

// wrap the native class for more flexibility :
function Xmm(modelType, options) {
  this._xmm = new XmmNative.xmm(modelType);
  // this._initialize(modelType);

  if (options) {
    this.setConfig(options);
  }
}

// Xmm.prototype._initialize = function(modelType) {
//   this._xmm = new XmmNative.xmm(modelType);
//   // expose all the native methods in the wrapper :
//   for (var i in methods /* this._xmm */) {
//     var method = methods[i];
//     if (!Xmm.prototype[method]) {
//       // console.log(method);
//       // this[method] = this._xmm[method].bind(this._xmm);
//       Xmm.prototype[method] = function(input) {
//         // console.log(method);
//         return this._xmm[method](input);
//       };
//     }
//   }
// }

// no use for this, keeping it for reference.
// for some reason every method has to be wrapped manually
// because Napi objects' methods cannot be inspected from js (see below) :/

var methods = [
  'addPhrase',
  'getPhrase',
  'getPhrasesOfLabel',
  'removePhrase',
  'removePhrasesOfLabel',
  'getTrainingSetSize',
  'getTrainingSetLabels',
  'getTrainnigSet',
  'setTrainnigSet',
  'addTrainnigSet',
  'clearTrainnigSet',
  'train',
  'cancelTraining',
  'getModel',
  'setModel',
  'getModelType',
  'reset',
  'filter',
]

Xmm.prototype.addPhrase = function(phrase) {
  return this._xmm.addPhrase(phrase);
}

Xmm.prototype.getPhrase = function(index = 0) {
  return this._xmm.getPhrase();
}

Xmm.prototype.getPhrasesOfLabel = function(label = "") {
  return this._xmm.getPhrasesOfLabel(label);
}

Xmm.prototype.removePhrase = function(index = -1) {
  return this._xmm.removePhrase(index);
}

Xmm.prototype.removePhrasesOfLabel = function(label = "") {
  return this._xmm.removePhrasesOfLabel(label);
}

Xmm.prototype.getTrainingSetSize = function() {
  return this._xmm.getTrainingSetSize();
}

Xmm.prototype.getTrainingSetLabels = function() {
  return this._xmm.getTrainingSetLabels();
}

Xmm.prototype.getTrainingSet = function() {
  return this._xmm.getTrainingSet();
}

Xmm.prototype.setTrainingSet = function(set) {
  return this._xmm.setTrainingSet(set);
}

Xmm.prototype.addTrainingSet = function(set) {
  return this._xmm.addTrainingSet(set);
}

Xmm.prototype.clearTrainingSet = function() {
  return this._xmm.clearTrainingSet();
}

Xmm.prototype.train = function(callback) {
  return this._xmm.train(callback);
}

Xmm.prototype.cancelTraining = function() {
  return this._xmm.cancelTraining();
}

Xmm.prototype.getModel = function() {
  return this._xmm.getModel();
}

Xmm.prototype.setModel = function(model) {
  return this._xmm.setModel(model);
}

Xmm.prototype.getModelType = function() {
  return this._xmm.getModelType();
}

Xmm.prototype.reset = function() {
  return this._xmm.reset();
}

Xmm.prototype.filter = function(observation) {
  return this._xmm.filter(observation);
}


// If we want to override some of the native methods, we could do this
// (the "if (!Xmm.prototype[method])" test avoids messing up the exposed native
// methods with the overriden ones) :

function translateFromXmmConfigProp(prop) {
  if (prop === 'relative_regularization') {
    return 'relativeRegularization';
  } else if (prop === 'absolute_regularization') {
    return 'absoluteRegularization';
  } else if (prop === 'covariance_mode') {
    return 'covarianceMode';
  } else if (prop === 'transition_mode') {
    return 'transitionMode';
  } else if (prop === 'regression_estimator') {
    return 'regressionEstimator';
  } else if (prop === 'multiClass_regression_estimator') {
    return 'multiClassRegressionEstimator';
  } else {
    return prop;
  }
}

function translateToXmmConfigProp(prop) {
  if (prop === 'relativeRegularization') {
    return 'relative_regularization';
  } else if (prop === 'absoluteRegularization') {
    return 'absolute_regularization';
  } else if (prop === 'covarianceMode') {
    return 'covariance_mode';
  } else if (prop === 'transitionMode') {
    return 'transition_mode';
  } else if (prop === 'regressionEstimator') {
    return 'regression_estimator';
  } else if (prop === 'multiClassRegressionEstimator') {
    return 'multiClass_regression_estimator';
  } else {
    return prop;
  }
}

Xmm.prototype.getConfig = function(prop) {
  if (prop) {
    var translatedProp = translateToXmmConfigProp(prop);
    return this._xmm.getConfig(translatedProp);
  } else {
    var config = this._xmm.getConfig();
    var outConfig = {};
    for (var prop in config) {
      var translatedProp = translateFromXmmConfigProp(prop);
      outConfig[translatedProp] = config[prop];
    }
    return outConfig;
  }
}

Xmm.prototype.setConfig = function(config) {
  var inConfig = {};
  for (var prop in config) {
    var translatedProp = translateToXmmConfigProp(prop);
    inConfig[translatedProp] = config[prop];
  }
  this._xmm.setConfig(inConfig);
}

Xmm.prototype.setModelType = function(modelType) {
  var options = this._xmm.getConfig();
  this._xmm = new XmmNative.xmm(modelType);

  this.setConfig(options);
}

// Xmm.prototype.setModel = function(model) {
//   this._xmm.setModel(JSON.parse(JSON.stringify(model)));
//   // this avoid rounding issues on config double parameters
//   this._xmm.setConfig(model.configuration.default_parameters);
//   // todo : how to avoid rounding issues on all double numbers ?
//   // answer : seems that we don't really care ...
// }

module.exports = Xmm;
