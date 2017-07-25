import xmm from '../index';
import test from 'tape';

test('basic', (t) => {
	let modelConfig = {
		gaussians: 3,
		covarianceMode: 'full',
		relativeRegularization: 0.1,
		absoluteRegularization: 0.1
	};

	const gmm = new xmm('gmm', modelConfig);

	const modelConfigMsg
		= 'model configuration should be the same as the one it\'s been instantiated with';

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

