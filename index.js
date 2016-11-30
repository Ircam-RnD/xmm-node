/*
 * This library is developed by the ISMM (http://ismm.ircam.fr/) team at IRCAM,
 * within the context of the RAPID-MIX (http://rapidmix.goldsmithsdigital.com/)
 * project, funded by the European Union’s Horizon 2020 research and innovation programme.  
 * Original XMM code authored by Jules Françoise, ported to Node.js by Joseph Larralde.  
 * See https://github.com/Ircam-RnD/xmm for detailed XMM credits.
 */

/** @module xmm */

// var Xmm = null;
var XmmNative = null;


/** @todo pre-build binaries for windows */

// Load the precompiled binary for windows.

// if(process.platform == "win32" && process.arch == "x64") {
//  xmm = require('./bin/winx64/xmm');  
// } else if(process.platform == "win32" && process.arch == "ia32") {
//  xmm = require('./bin/winx86/xmm');  
// } else {

// Load the new built binary for other platforms.
// Xmm = require('./build/Release/xmm');
XmmNative = require('./build/Release/xmm');

//}

// wrap the native class for more flexibility :
function Xmm(modelType, options) {
  this._xmm = new XmmNative(modelType);

  // expose all the native methods in the wrapper :
  for (var method in this._xmm) {
    if (!Xmm.prototype[method]) {
      this[method] = this._xmm[method].bind(this._xmm);
    }
  }

  if (options) {
    this.setConfig(options);
  }
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

// Xmm.prototype.setModel = function(model) {
//   this._xmm.setModel(JSON.parse(JSON.stringify(model)));
//   // this avoid rounding issues on config double parameters
//   this._xmm.setConfig(model.configuration.default_parameters);
//   // todo : how to avoid rounding issues on all double numbers ?
//   // answer : seems that we don't really care ...
// }

module.exports = Xmm;
