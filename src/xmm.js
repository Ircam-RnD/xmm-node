// var xmm = require('bindings')('xmm');

// var gmm = new xmm('gmm', {});
// var hhmm = new xmm('hhmm', {});

// var binary = require('node-pre-gyp');
// var path = require('path');
// var bindingPath = binary.find(path.resolve(path.join(__dirname, '../package.json')));

// var xmm = require(bindingPath);
// var o = new xmm.xmm('hhmm');

var xmm = require('../index');
var o = new xmm('hhmm');

console.log(o.getPhrasesOfLabel("label"));
console.log(o.getModelType());
console.log(o.getTrainingSetSize());
o.train(function(err, res) {
  console.error(`error : ${err}`);
  console.log(`result : ${JSON.stringify(res, null, 2)}`);
  o.filter([0]);
});
// o.cancelTraining();
console.log('training');

// console.log( obj.plusOne() ); // 11
// console.log( obj.plusOne() ); // 12
// console.log( obj.plusOne() ); // 13

// console.log( obj.multiply().value() ); // 13
// console.log( obj.multiply(10).value() ); // 130

// var newobj = obj.multiply(-1);
// console.log( newobj.value() ); // -13
// console.log( obj === newobj ); // false