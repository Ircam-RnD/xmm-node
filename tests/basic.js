import Xmm from '../index';
import test from 'tape';

test('basic', (t) => {
	const gmm = new Xmm('gmm');

	const modelConfig = gmm.getConfig();
	gmm.setConfig(modelConfig);
	const modelConfigMsg = 'model configuration should not change when replaced by itself';
	t.deepEqual(gmm.getConfig(), modelConfig, modelConfigMsg);

	t.end();
});
