var xmm = require('./lib/index.js');
//var xmm = require('./build/Release/xmm');

var phrase = {
	bimodal: false,
	dimension: 2,
	dimension_input: 0,
	length: 4,
	data: [0, 1, 2, 3, 4, 5, 6, 7],
	column_names: ["bubu", "bibi", "boubou"],
	label: 'minitest'
};

var gmm = new xmm('gmmm');
gmm = new xmm('gmm');
// var hhmm = new xmm('hhmm');

var hhmm = new xmm({
	model: 'hhmm',
	gaussians: 5,
	covariance_mode: 'diagonal',
	states: 10
});
//xmm.machin = { a: 1 };


gmm.config = {
	states: 10,
	gaussians: 2,
	relative_regularization: 0.2
};

console.log(phrase);

for(var i=0; i<1000; i++) {
	//console.log('added new phrase at index : ' + gmm.addPhrase(phrase));
	gmm.addPhrase(phrase)
}
console.log('\n');
console.log('trying to add invalid phrase returns : ' + gmm.addPhrase(5));

console.log('training set size : ' + gmm.getTrainingSetSize());

// console.log('\n');
// console.log(gmm.getTrainingSet().phrases[0].column_names);

console.log('\n');
console.log(gmm.getPhrase(999));

//*
for(var i=0; i<1; i++) {
	gmm.train(function(msg) {
		//console.log(msg);
		console.log(gmm.getModel().models[0].components);
		//console.log(gmm.getTrainingSet());
	});
	//gmm.addPhrase(phrase);
}
//*/

//console.log('aha !');


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