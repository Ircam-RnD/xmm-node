## xmm-node
### XMM native addon for Node.js

Wraps the [XMM](https://github.com/julesfrancoise/xmm) library into a Node.js module.
Can be used together with the client-side model decoders library
[xmm-client](https://github.com/Ircam-RnD/xmm-client).

#### developers :

install :
- install latest Node.js version
- install node-gyp : `npm install -g node-gyp`
- clone xmm-node : `git clone --recursive https://github.com/Ircam-RnD/xmm-node.git`
- install dependencies listed in `package.json` : `npm install`

build : `node-gyp configure build`

<!--
#### Test :
`node test/xmmtest.js`
-->

#### users :

`npm install Ircam-RnD/xmm-node [--save]`

#### note :

this module is a work in progress, use at your own risk

#### API documentation :

ERROR, Cannot find module.<a name="xmm"></a>

## xmm
**Kind**: global class  

* [xmm](#xmm)
    * [new xmm([modelType], [modelCongig])](#new_xmm_new)
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
    * [.train(trainCallback)](#xmm+train)
    * [.getModel()](#xmm+getModel) ⇒ <code>Object</code>
    * [.getModelType()](#xmm+getModelType) ⇒ <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code>
    * [.reset()](#xmm+reset)
    * [.filter(observation)](#xmm+filter) ⇒ <code>Object</code>
    * [.getConfig([configParam])](#xmm+getConfig) ⇒ <code>[XmmModelConfig](#XmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code>
    * [.setConfig(config)](#xmm+setConfig)

<a name="new_xmm_new"></a>

### new xmm([modelType], [modelCongig])
The main xmm class.


| Param | Type | Default | Description |
| --- | --- | --- | --- |
| [modelType] | <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code> | <code>&#x27;gmm&#x27;</code> | The type of model. |
| [modelCongig] | <code>[XmmModelConfig](#XmmModelConfig)</code> |  | Configuration parameters for the model. |

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

### xmm.train(trainCallback)
Trains the model with the current training set.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| trainCallback | <code>[TrainCallback](#TrainCallback)</code> | The callback handling the trained model. |

<a name="xmm+getModel"></a>

### xmm.getModel() ⇒ <code>Object</code>
Returns the trained model (the same object as in [TrainCallback](#TrainCallback)).

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>Object</code> - An object containing the trained model.  
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

<a name="xmm+getConfig"></a>

### xmm.getConfig([configParam]) ⇒ <code>[XmmModelConfig](#XmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code>
Get the actual model configuration parameters or one of them.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>[XmmModelConfig](#XmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code> - If called without any argument :
<li style="list-style-type: none;">
<ul> an object of type [XmmModelConfig](#XmmModelConfig) containing all the actual model configuration parameters</ul>
</li>
Otherwise, the returned value type depends on the requested configuration parameter :
<li style="list-style-type: none;">
<ul>'gaussians' : the number of gaussians</ul>
<ul>'relative_regularization' : the relative regularization foat value</ul>
<ul>'absolute_regularization' : the absolute regularization float value</ul>
<ul>'covariance_mode' : the actual covariance mode ('full' or 'diagonal')</ul>
<ul>'hierarchical' : if the model type is not 'hhmm', <code>undefined</code>, otherwise
	<code>true</code> if the model is hierarchical and <code>false</code> if it is not</ul>
<ul>'states' : if the model type is not 'hhmm', <code>undefined</code>, otherwise
	the number of states of the hmms</ul>
<ul>'transition_mode' : if the model type is not 'hhmm', <code>undefined</code>, otherwise
	the actual transition mode value ('ergodic' or 'leftright')</ul>
<ul>'regression_estimator' : if the model type is not 'hhmm', <code>undefined</code>, otherwise
	the actual regression estimator used with hmms ('full', 'windowed' or 'likeliest')</ul>
</li>  

| Param | Type | Description |
| --- | --- | --- |
| [configParam] | <code>&#x27;gaussians&#x27;</code> &#124; <code>&#x27;relative_regularization&#x27;</code> &#124; <code>&#x27;absolute_regularization&#x27;</code> &#124; <code>&#x27;covariance_mode&#x27;</code> &#124; <code>&#x27;hierarchical&#x27;</code> &#124; <code>&#x27;states&#x27;</code> &#124; <code>&#x27;transition_mode&#x27;</code> &#124; <code>&#x27;regression_estimator&#x27;</code> | The name of a configuration parameter. |

<a name="xmm+setConfig"></a>

### xmm.setConfig(config)
Set the actual model configuration parameters.

**Kind**: instance method of <code>[xmm](#xmm)</code>  

| Param | Type | Description |
| --- | --- | --- |
| config | <code>[XmmModelConfig](#XmmModelConfig)</code> | A config object as returned by [getConfig](getConfig) when called without arguments (missing parameters won't be changed internally and invalid ones will be ignored). |

