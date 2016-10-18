import xmm from '../index';
import test from 'tape';

test('basic', (t) => {
	const gmm = new xmm('gmm');

	const modelConfig = gmm.getConfig();
	gmm.setConfig(modelConfig);
	// console.log(modelConfig);
	const modelConfigMsg = 'model configuration should not change when replaced by itself';
	t.deepEqual(gmm.getConfig(), modelConfig, modelConfigMsg);

	t.end();
});

test('empty', (t) => {
	const hhmm = new xmm('hmm', {
		gaussians: 3,
		relative_regularization: 0.1,
		absolute_regularization: 0.1,
		covariance_mode: 'diagonal',
		hierarchical: false,
		states: 10,
		transition_mode: 'ergodic',
		regression_estimator: 'likeliest',
		dummy_field: 'nawak'
	});
	// console.log(hhmm.getConfig());
	// console.log(hhmm.filter([0, 0]));
	const emptyLikelihoodsMsg = 'untrained model should return empty likelihoods on filter';
	t.equal(hhmm.filter([0,0]).smoothed_log_likelihoods.length, 0, emptyLikelihoodsMsg);
	t.end();
});

test('training', (t) => {
	const hhmm = new xmm('hhmm', {
		hierarchical: false
	});
	// console.log(hhmm.getConfig());

	hhmm.addPhrase({
		bimodal: false,
		dimension: 2,
		dimension_input: 0,
		data: [
			1.1, 1.2,
			2.3, 2.1,
			3.7, 3.2
		],
		data_input: [],
		data_output: [],
		length: 3,
		label: 'aLabel'
	});
	hhmm.train((err, res) => {
		t.notEqual(res, null || undefined, 'train should return a trained model');
		// console.log(hhmm.filter([1, 1]));
	})

	t.end();
});
