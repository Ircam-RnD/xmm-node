## xmm-node
### XMM native addon for Node.js

Wraps the [XMM](https://github.com/julesfrancoise/xmm) library into a Node.js module.
Can be used together with the client-side model decoders library
[xmm-client](https://github.com/Ircam-RnD/xmm-client).

#### developers :

install :
- install latest Node.js version
- install node-gyp : `npm install -g node-gyp`
- clone xmm-node : `git clone --recursive https://github.com/Ircam-RnD/xmm-node.git`
- install dependencies listed in `package.json` : `npm install`

build : `node-gyp configure build`

<!--
#### Test :
`node test/xmmtest.js`
-->

#### users :

`npm install Ircam-RnD/xmm-node [--save]`

#### note :

this module is a work in progress, use at your own risk
