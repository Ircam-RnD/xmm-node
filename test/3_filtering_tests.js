import xmm from '../index';
import { GmmDecoder, SetMaker } from 'xmm-client';
import test from 'tape';

test('filtering input format checking', (t) => {
  const setMaker = new SetMaker();

  const gmm = new xmm('gmm', {
    // hierarchical: false,
    relativeRegularization: 0.1,
    multiClassRegressionEstimator: 'mixture',
  });

  var p = {
    bimodal: true,
    dimension: 6,
    dimension_input: 3,
    column_names: [ "" ],
    data_input: [1, 2, 3],
    data_output: [0, 0, 0],
    length: 1,
    label: 'aLabel'
  };

  for (let i = 0; i < 5; i++) {
    setMaker.addPhrase(JSON.parse(JSON.stringify(p)));
  }

  p.data_input = [3, 2, 1];
  p.data_output = [10, 10, 10];
  p.label = 'anotherLabel';

  for (let i = 0; i < 5; i++) {
    setMaker.addPhrase(JSON.parse(JSON.stringify(p)));
  }

  const set = setMaker.getTrainingSet();
  gmm.setTrainingSet(set);

  const filteringWithEmptyInputMsg = 'filtering an empty array should return null';

  gmm.train((err, res) => {
    const f = gmm.filter([]);

    t.equal(f, null, filteringWithEmptyInputMsg);
    t.end();
  });
});