# XMM native addon for NodeJS

Wraps [XMM](https://github.com/julesfrancoise/xmm) library into a NodeJS module

## Develop :

### Prerequisites :
- install latest NodeJS version
- install node-gyp : `npm install -g node-gyp`
- install dependencies listed in `package.json` : `npm install`

### Build :
`node-gyp configure build`

### Test :
`node test/xmmtest.js`

## Consume :

do none of the above, simply add `"xmm": "github:ircam-rnd/xmm-node"` to your dependencies in `package.json`, then `npm install`

## Note

this module is a work in progress, use at your own risk
