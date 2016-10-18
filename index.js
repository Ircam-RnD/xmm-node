/** @module xmm */

// var Xmm = null;
var XmmNative = null;


/** @todo pre-build binaries for windows */

// Load the precompiled binary for windows.

// if(process.platform == "win32" && process.arch == "x64") {
// 	xmm = require('./bin/winx64/xmm');  
// } else if(process.platform == "win32" && process.arch == "ia32") {
// 	xmm = require('./bin/winx86/xmm');  
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

// Xmm.prototype.getConfig = function(arg) {
// 	if (arg) {
// 		return this._xmm.getConfig(arg);
// 	} else {
// 		return this._xmm.getConfig();
// 	}
// }

module.exports = Xmm;
