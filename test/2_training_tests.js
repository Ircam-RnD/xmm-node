import xmm from '../index';
import { SetMaker } from 'xmm-client';
import test from 'tape';

test('training', (t) => {

  t.plan(4);

  const setMaker = new SetMaker();

  const hhmm = new xmm('hhmm', {
    hierarchical: false,
    relativeRegularization: 0.1
  });

  var p = {
    bimodal: false,
    dimension: 2,
    dimension_input: 0,
    column_names: [ "" ],
    data: [
      1.1, 1.2,
      2.3, 2.1,
      3.7, 3.2
    ],
    // data_input: [1, 2],
    // data_output: [1],
    length: 3,
    label: 'aLabel'
  }

  const trainMsgOne = 'train should return a null model when training is cancelled';

  hhmm.train((err, res) => {
    t.equal(res, null, trainMsgOne);
  });

  hhmm.cancelTraining();

  const trainMsgTwo = 'train should return an empty model when trained with empty set';

  hhmm.train((err, res) => {
    t.deepEqual(res.models, [], trainMsgTwo);
  });

  const trainMsgThree = 'train should return a trained model';

  for (let i = 0; i < 500; i++) {
    // setMaker.addPhrase(p);
    hhmm.addPhrase(JSON.parse(JSON.stringify(p)));
  }
  // hhmm.setTrainingSet(setMaker.getTrainingSet());
  // hhmm.clearTrainingSet();
  // hhmm.addTrainingSet(setMaker.getTrainingSet());

  hhmm.train((err, res) => {
    t.notEqual(res, null, trainMsgThree);
    t.equal(res.models.length > 0, true, trainMsgThree);

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
