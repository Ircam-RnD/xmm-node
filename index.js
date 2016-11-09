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
  this._xmm.setConfig(options);

  // expose all the native methods in the wrapper :
  for (var method in this._xmm) {
    if (!Xmm.prototype[method]) {
      this[method] = this._xmm[method].bind(this._xmm);
    }
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
    const translatedProp = translateToXmmConfigProp(prop);
    return this._xmm.getConfig(translatedProp);
  } else {
    const config = this._xmm.getConfig();
    for (let prop in config) {
      const translatedProp = translateFromXmmConfigProp(prop);
      config[translatedProp] = config[prop];
      delete config[prop];
    }
    return config;
  }
}

Xmm.prototype.setConfig = function(config) {
  for (let prop in config) {
    const translatedProp = translateToXmmConfigProp(prop);    
    config[translatedProp] === config[prop];
    delete config[prop];
  }
  this._xmm.setConfig(config);
}

module.exports = Xmm;
