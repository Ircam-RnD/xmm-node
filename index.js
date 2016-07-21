/** @module xmm */

var xmm = null;

/** @todo pre-build binaries for windows */

/*
// Load the precompiled binary for windows.
if(process.platform == "win32" && process.arch == "x64") {
	xmm = require('./bin/winx64/xmm');  
} else if(process.platform == "win32" && process.arch == "ia32") {
	xmm = require('./bin/winx86/xmm');  
} else {
//*/
	// Load the new built binary for other platforms.
	xmm = require('./build/Release/xmm');  
//}

module.exports = xmm;
