# xmm-node
### XMM native addon for Node.js

Wraps the [XMM](https://github.com/ircam-rnd/xmm) library into a Node.js module.
Can be used together with the client-side model decoders library
[xmm-client](https://github.com/ircam-rnd/xmm-client).

#### developers :

install :
- install latest Node.js version
- install node-gyp : `npm install -g node-gyp`
- clone xmm-node : `git clone --recursive https://github.com/ircam-rnd/xmm-node.git`
- install dependencies listed in `package.json` : `npm install`

build : `node-gyp configure build`

test : `npm run tests`

#### users :

`npm install [--save] ircam-rnd/xmm-node`

example :

```JavaScript
// es6 :
import xmm from 'xmm-node';
// or older es versions :
var xmm = require('xmm-node');

// then :
var hhmm = new xmm('hhmm', {
	gaussians: 3,
	states: 12,
	relativeRegularization: 0.1,
	absoluteRegularization: 0.1
});

// valid xmm phrase created with xmm-client/PhraseMaker
var phrase = someFunctionReturningAPhrase();

hhmm.addPhrase(phrase);

hhmm.train(function(err, res) {
	if (err === null) {
		// res is a trained model : pass it to xmm-client/HhmmDecoder
		// or use hhmm.filter(someObservationVector) to do the decoding server-side.
	}
});
```

#### credits :

This library is developed by the [ISMM](http://ismm.ircam.fr/) team at IRCAM,
within the context of the [RAPID-MIX](http://rapidmix.goldsmithsdigital.com/)
project, funded by the European Union’s Horizon 2020 research and innovation programme.  
Original XMM code authored by Jules Françoise, ported to Node.js by Joseph Larralde.  
See [github.com/Ircam-RnD/xmm](https://github.com/Ircam-RnD/xmm) for detailed XMM credits.

<hr>

# API documentation :

<a name="xmm"></a>

## xmm
**Kind**: global class  

* [xmm](#xmm)
    * [new xmm([modelType], [modelConfig])](#new_xmm_new)
    * [.getConfig([configParam])](#xmm+getConfig) ⇒ <code>[xmmModelConfig](#xmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code>
    * [.setConfig(config)](#xmm+setConfig)
    * [.addPhrase(phrase)](#xmm+addPhrase)
    * [.getPhrase(index)](#xmm+getPhrase) ⇒ <code>Object</code>
    * [.getPhrasesOfLabel(label)](#xmm+getPhrasesOfLabel) ⇒ <code>Object</code>
    * [.removePhrase(index)](#xmm+removePhrase)
    * [.removePhrasesOfLabel(label)](#xmm+removePhrasesOfLabel)
    * [.getTrainingSetSize()](#xmm+getTrainingSetSize) ⇒ <code>Number</code>
    * [.getTrainingSetLabels()](#xmm+getTrainingSetLabels) ⇒ <code>Array.String</code>
    * [.getTrainingSet()](#xmm+getTrainingSet) ⇒ <code>Object</code>
    * [.setTrainingSet(trainingSet)](#xmm+setTrainingSet)
    * [.addTrainingSet(trainingSet)](#xmm+addTrainingSet)
    * [.clearTrainingSet()](#xmm+clearTrainingSet)
    * [.train(callback)](#xmm+train)
    * [.cancelTraining()](#xmm+cancelTraining)
    * [.getModel()](#xmm+getModel) ⇒ <code>Object</code>
    * [.setModel(model)](#xmm+setModel)
    * [.getModelType()](#xmm+getModelType) ⇒ <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code>
    * [.reset()](#xmm+reset)
    * [.filter(observation)](#xmm+filter) ⇒ <code>Object</code>

<a name="new_xmm_new"></a>

### new xmm([modelType], [modelConfig])
The main xmm class.


| Param | Type | Default | Description |
| --- | --- | --- | --- |
| [modelType] | <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code> | <code>&#x27;gmm&#x27;</code> | The type of model. |
| [modelConfig] | <code>[xmmModelConfig](#xmmModelConfig)</code> |  | Configuration parameters for the model. |

<a name="xmm+getConfig"></a>

### xmm.getConfig([configParam]) ⇒ <code>[xmmModelConfig](#xmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code>
Get the actual model configuration parameters or one of them.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>[xmmModelConfig](#xmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code> - Depends on the parameter.
<br /><br />If called without any argument, returns :
<li style="list-style-type: none;">
<ul> an object of type <code>xmmModelConfig</code> containing all the actual model configuration parameters</ul>
</li>
Otherwise, the returned value type depends on the requested configuration parameter :
<li style="list-style-type: none;">
<ul><code>'gaussians'</code> : the number of gaussians</ul>
<ul><code>'relativeRegularization'</code> : the relative regularization foat value</ul>
<ul><code>'absoluteRegularization'</code> : the absolute regularization float value</ul>
<ul><code>'covarianceMode'</code> : the actual covariance mode ('full' or 'diagonal')</ul>
<ul><code>'hierarchical'</code> : if the model type is not 'hhmm', <code>undefined</code>,
otherwise <code>true</code> if the model is hierarchical and <code>false</code> if it is not</ul>
<ul><code>'states'</code> : if the model type is not 'hhmm', <code>undefined</code>, otherwise
	the number of states of the hmms</ul>
<ul><code>'transitionMode'</code> : if the model type is not 'hhmm', <code>undefined</code>,
otherwise the actual transition mode value ('ergodic' or 'leftright')</ul>
<ul><code>'regressionEstimator'</code> : if the model type is not 'hhmm',
<code>undefined</code>, otherwise the actual regression estimator used with hmms
('full', 'windowed' or 'likeliest')</ul>
</li>  

| Param | Type | Description |
| --- | --- | --- |
| [configParam] | <code>&#x27;gaussians&#x27;</code> &#124; <code>&#x27;relativeRegularization&#x27;</code> &#124; <code>&#x27;absoluteRegularization&#x27;</code> &#124; <code>&#x27;covarianceMode&#x27;</code> &#124; <code>&#x27;hierarchical&#x27;</code> &#124; <code>&#x27;states&#x27;</code> &#124; <code>&#x27;transitionMode&#x27;</code> &#124; <code>&#x27;regressionEstimator&#x27;</code> | The name of a configuration parameter. |

<a name="xmm+setConfig"></a>

### xmm.setConfig(config)
Set the actual model configuration parameters.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| config | <code>[xmmModelConfig](#xmmModelConfig)</code> | A config object as returned by [getConfig](getConfig) when called without arguments (missing parameters won't be changed internally and invalid ones will be ignored). |

<a name="xmm+addPhrase"></a>

### xmm.addPhrase(phrase)
Add a phrase to the internal training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| phrase | <code>Object</code> | An object containing a valid XMM phrase. |

<a name="xmm+getPhrase"></a>

### xmm.getPhrase(index) ⇒ <code>Object</code>
Get phrase of a certain index from the internal training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Object</code> - A valid XMM phrase from the internal training set.  

| Param | Type | Description |
| --- | --- | --- |
| index | <code>Number</code> | The index of a phrase in the training set. |

<a name="xmm+getPhrasesOfLabel"></a>

### xmm.getPhrasesOfLabel(label) ⇒ <code>Object</code>
Get phrases of a certain label from the internal training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Object</code> - A valid XMM training set containing all the requested phrases.  

| Param | Type | Description |
| --- | --- | --- |
| label | <code>String</code> | The label of which we want to get the phrases. |

<a name="xmm+removePhrase"></a>

### xmm.removePhrase(index)
Remove phrase of a certain index from the internal training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| index | <code>Number</code> | The index of a phrase in the training set. |

<a name="xmm+removePhrasesOfLabel"></a>

### xmm.removePhrasesOfLabel(label)
Remove phrases of a certain label from the internal training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| label | <code>String</code> | The label of which we want to remove the phrases. |

<a name="xmm+getTrainingSetSize"></a>

### xmm.getTrainingSetSize() ⇒ <code>Number</code>
Get the number of phrases in the training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Number</code> - The number of phrases in the training set.  
<a name="xmm+getTrainingSetLabels"></a>

### xmm.getTrainingSetLabels() ⇒ <code>Array.String</code>
Get the array of all the labels in the training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Array.String</code> - An array containing all the training set's labels.  
<a name="xmm+getTrainingSet"></a>

### xmm.getTrainingSet() ⇒ <code>Object</code>
Get the actual training set as an object.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Object</code> - An object containing a valid XMM training set.  
<a name="xmm+setTrainingSet"></a>

### xmm.setTrainingSet(trainingSet)
Sets the actual training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| trainingSet | <code>Object</code> | An object containing a valid XMM training set. |

<a name="xmm+addTrainingSet"></a>

### xmm.addTrainingSet(trainingSet)
Adds a training set to the actual training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| trainingSet | <code>Object</code> | An object containing a valid XMM training set. |

<a name="xmm+clearTrainingSet"></a>

### xmm.clearTrainingSet()
Clears the training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
<a name="xmm+train"></a>

### xmm.train(callback)
Trains the model with the current training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| callback | <code>[trainCallback](#trainCallback)</code> | The callback handling the trained model. |

<a name="xmm+cancelTraining"></a>

### xmm.cancelTraining()
Cancel the current training process. <br />
<i><strong><font color="#f00">WARNING</font></strong>
This feature is experimental and may cause crashes</i>

**Kind**: instance method of <code>[xmm](#xmm)</code>  
<a name="xmm+getModel"></a>

### xmm.getModel() ⇒ <code>Object</code>
Returns the trained model (the same object as in [trainCallback](#trainCallback)).

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Object</code> - An object containing the trained model.  
<a name="xmm+setModel"></a>

### xmm.setModel(model)
Sets the actual model from another already trained model.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| model | <code>Object</code> | A valid XMM model of the instance's actual type. |

<a name="xmm+getModelType"></a>

### xmm.getModelType() ⇒ <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code>
Returns the type of the actual model.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code> - The type of model as String.  
<a name="xmm+reset"></a>

### xmm.reset()
Resets the internal variables used for filtering.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
<a name="xmm+filter"></a>

### xmm.filter(observation) ⇒ <code>Object</code>
Estimates an input array of floats.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Object</code> - filteringResults - An object containing the estimation results.  

| Param | Type | Description |
| --- | --- | --- |
| observation | <code>Array.Number</code> | The observation we want an estimation of. |

<hr>
<a name="xmmModelConfig"></a>

## xmmModelConfig
**Kind**: global typedef  
**Properties**

| Name | Type | Default | Description |
| --- | --- | --- | --- |
| gaussians | <code>Number</code> | <code>1</code> | the number of gaussians used for encoding a state. |
| relativeRegularization | <code>Number</code> | <code>0.01</code> | the relative regularization (see XMM documentation). |
| absoluteRegularization | <code>Number</code> | <code>0.01</code> | the absolute regularization (see XMM documentation). |
| covarianceMode | <code>&#x27;diagonal&#x27;</code> &#124; <code>&#x27;full&#x27;</code> | <code>&#x27;full&#x27;</code> | the type of covariance matrix used in the model. |
| hierarchical | <code>Boolean</code> | <code>true</code> | if <code>model</code> is 'hhmm', turns hierarchical mode on/off. |
| states | <code>Number</code> | <code>1</code> | if <code>model</code> is 'hhmm', defines the number of states used to generate each individual hmm. |
| transitionMode | <code>&#x27;ergodic&#x27;</code> &#124; <code>&#x27;leftright&#x27;</code> | <code>&#x27;leftright&#x27;</code> | if <code>model</code> is 'hhmm', sets the transition mode between the states of the individual hmm models. |
| regressionEstimator | <code>&#x27;full&#x27;</code> &#124; <code>&#x27;windowed&#x27;</code> &#124; <code>&#x27;likeliest&#x27;</code> | <code>&#x27;full&#x27;</code> | if <code>model</code> is 'hhmm', the type of estimator used for regression with hmms. |

<hr>
<a name="trainCallback"></a>

## trainCallback : <code>function</code>
Callback handling the trained model.

**Kind**: global typedef  

| Param | Type | Description |
| --- | --- | --- |
| err | <code>String</code> | Description of a potential error. |
| res | <code>Object</code> | An object containing the trained model. |

<hr>
