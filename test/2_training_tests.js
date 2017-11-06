import xmm from '../index';
import { GmmDecoder, SetMaker } from 'xmm-client';
import test from 'tape';

test('training', (t) => {

  t.plan(4);

  const setMaker = new SetMaker();

  const gmm = new xmm('gmm', {
    // hierarchical: false,
    relativeRegularization: 0.1,
    multiClassRegressionEstimator: 'mixture',
  });

  const gmmClient = new GmmDecoder();

  var p = {
    bimodal: true,
    dimension: 6,
    dimension_input: 3,
    column_names: [ "" ],
    // data: [
    //   1.1, 1.2,
    //   2.3, 2.1,
    //   3.7, 3.2
    // ],
    data_input: [1, 2, 3],
    data_output: [0, 0, 0],
    length: 1,
    label: 'aLabel'
  };

  const trainMsgOne = 'train should return a null model when training is cancelled';

  gmm.train((err, res) => {
    t.equal(res, null, trainMsgOne);
  });

  gmm.cancelTraining();

  const trainMsgTwo = 'train should return an empty model when trained with empty set';

  gmm.train((err, res) => {
    t.deepEqual(res.models, [], trainMsgTwo);
  });

  const trainMsgThree = 'train should return a trained model';

  for (let i = 0; i < 5; i++) {
    setMaker.addPhrase(JSON.parse(JSON.stringify(p)));
    // hhmm.addPhrase(JSON.parse(JSON.stringify(p)));
  }

  p.data_input = [3, 2, 1];
  p.data_output = [10, 10, 10];
  p.label = 'anotherLabel';

  for (let i = 0; i < 5; i++) {
    setMaker.addPhrase(JSON.parse(JSON.stringify(p)));
    // hhmm.addPhrase(JSON.parse(JSON.stringify(p)));
  }

  const set = setMaker.getTrainingSet();
  gmm.setTrainingSet(set);
  console.log(set);
  // hhmm.clearTrainingSet();
  // hhmm.addTrainingSet(setMaker.getTrainingSet());

  gmm.train((err, res) => {
    console.log(JSON.stringify(res, null, 2));
    gmmClient.setModel(res);

    t.notEqual(res, null, trainMsgThree);
    t.equal(res.models.length > 0, true, trainMsgThree);

    console.log(gmmClient.filter([3, 2, 3]));
    console.log(gmmClient.filter([3, 2, 1]));

    // const setModelConfigMsg = 'config should not change when queried after setModel';
    // let config = hhmm.getConfig();
    // hhmm.setModel(res);
    // console.log(hhmm.getConfig());
    // console.log(hhmm.getModel()['models']);
    // doesn't work : ROUNDING ISSUES IN JSONCPP ???? (happen during setModel)
    // t.deepEqual(config, hhmm.getConfig(), setModelConfigMsg)
  });

  // const trainMsgFour = 'train should return an empty model when training set empty';
  // hhmm.clearTrainingSet();
  // hhmm.train((err, res) => {
  //   t.notEqual(res, null, trainMsgFour);
  // });

  // t.end();
});
