/*
 * really dirty testing script
 *
 */

var Xmm = require('../index.js');
var util = require('util');

var prettify = function(toPrint) {
	return util.inspect(toPrint, false, null);
}

var phrase = {
	bimodal: false,
	dimension: 2,
	dimension_input: 0,
	length: 4,
	data: [0, 1, 2, 3, 4, 5, 6, 7],
	column_names: ["bubu", "bibi", "boubou"],
	label: 'minitest'
};

// var gmm = new Xmm('gmm', {
// 	gaussians: 2
// });
// var gmm = new Xmm('gmm');
var gmm = new Xmm();

console.log('gmm nb of gaussians : ' + gmm.getConfig('gaussians'));
console.log('\n');
gmm = new Xmm('gmm');
// var hhmm = new Xmm('hhmm');

var hhmm = new Xmm('hhmm', {
	// model: 'hhmm',
	gaussians: 1,
	covariance_mode: 'diagonal',
	transition_mode: 'ergodic',
	hierarchical: true,
	states: 3
});
console.log('hhmm config : ' + prettify(hhmm.getConfig()));
//Xmm.machin = { a: 1 };


gmm.setConfig({
	states: 10,
	gaussians: 2,
	relative_regularization: 0.2
});

const states = gmm.getConfig('states');

console.log(phrase);

for(var i=0; i<100; i++) {
	//console.log('added new phrase at index : ' + gmm.addPhrase(phrase));
	gmm.addPhrase(phrase)
}

console.log('\n');
console.log('trying to add invalid phrase returns : ' + gmm.addPhrase(5));

console.log('training set size : ' + gmm.getTrainingSetSize());

// console.log('\n');
// console.log(gmm.getTrainingSet().phrases[0].column_names);

hhmm.setTrainingSet(gmm.getTrainingSet());

console.log('\n');
console.log(gmm.getPhrase(9));

//*
for(var i=0; i<1; i++) {
	gmm.train(function(err, msg) {
		console.log('training error : ' + err);
		console.log('model : ' + prettify(msg));
		//console.log(gmm.getModel().models[0].components);
		//console.log(gmm.getModel());
		//console.log(hhmm.getModel());
		//console.log(gmm.filter([0, 1]));
		//console.log(util.inspect(gmm.getModel(), false, null));
		//console.log(gmm.getTrainingSet());
	});
	//gmm.addPhrase(phrase);
}
//*/

//console.log('\n');
//console.log(hhmm.getTrainingSet());
//console.log(gmm.getPhrasesOfLabel('minitest'));
//console.log(gmm.getTrainingSetLabels());
//console.log('aha !');

//gmm.removePhrasesOfLabel('minitest');
//console.log(gmm.getTrainingSetSize());


// console.log(gmm.getTrainingSetSize());
// gmm.removePhrase(0);
// console.log(gmm.getTrainingSetSize());
// gmm.addPhrase(phrase);
// console.log(gmm.getTrainingSetSize());
// gmm.removePhrase(0);
// console.log(gmm.getTrainingSetSize());
// gmm.removePhrasesOfClass('minitest');
// console.log(gmm.getTrainingSetSize());

// console.log(gmm.getPhrase(10));

/*
hhmm.config = {
	covariance_mode: 'full',
	gaussians: 10
}
*/

//console.log(gmm.config);
//console.log(hhmm.config);

// console.log(gmm);
// console.log(hhmm);

/*
	regression_estimator : 'full', 'windowed', or 'likeliest'
	transition_mode : 'leftright' or 'ergodic'
	covariance_mode: 'full' or 'diagonal'
*/