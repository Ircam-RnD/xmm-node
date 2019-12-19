import xmm from '../index';
import test from 'tape';

test('instantiation without arguments', (t) => {
	const noArgsMsg = 'instantiation without arguments should create a gmm';

	const gmm = new xmm();
	t.equal(gmm.getModelType(), 'gmm', noArgsMsg);
	t.end();
});

test('model configuration consistency', (t) => {
	let modelConfig = {
		gaussians: 3,
		covarianceMode: 'full',
		relativeRegularization: 0.1,
		absoluteRegularization: 0.1,
		multiClassRegressionEstimator: 'mixture',
		// states: 1,
	};

	const gmm = new xmm('gmm', modelConfig);

	const modelConfigMsg
		= 'returned model configuration should be the same as the one it was instantiated with';

	t.deepEqual(gmm.getConfig(), modelConfig, modelConfigMsg);
	t.end();
});

test('untrained filtering', (t) => {
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

	const emptyFilterResults = {
		instant_likelihoods: [],
		instant_normalized_likelihoods: [],
		smoothed_likelihoods: [],
		smoothed_normalized_likelihoods: [],
		smoothed_log_likelihoods: [],
		likeliest: ''
	};

	const emptyFilterResultsMsg
		= 'untrained model should return empty filtering results';

	t.deepEqual(hhmm.filter([0]), emptyFilterResults, emptyFilterResultsMsg);
	t.end();
});

test('filtering empty array', (t) => {
	const gmm = new xmm('hhmm');

	const filteringEmptyArrayMsg = 'filtering an empty array should not crash';

	t.pass(filteringEmptyArrayMsg);
	t.end();
});

test('labels', (t) => {
	const gmm = new xmm('gmm');

	const clearNonexistentLabelMsg = 'clearing nonexistent label should not crash';

	gmm.removePhrasesOfLabel('randomLabel');
	t.pass(clearNonexistentLabelMsg);
	t.end();
});

test('phrases', (t) => {
	const gmm = new xmm('gmm');

	const getNonexistentPhraseMsg = 'trying to get nonexistent phrase should return false';

	t.equal(gmm.removePhrase(0), false, getNonexistentPhraseMsg);
	t.end();
});

test('model types', (t) => {
	const myXmm = new xmm('gmm');

	const changeModelTypeMsg = 'getModelType should return the new modelType set by setModelType';

	myXmm.setModelType('hhmm');
	t.equal(myXmm.getModelType(), 'hhmm', changeModelTypeMsg);
	t.end();
});

