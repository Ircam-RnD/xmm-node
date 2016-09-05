
# API documentation :

<a name="xmm"></a>

## xmm
**Kind**: global class  

* [xmm](#xmm)
    * [new xmm([modelType], [modelConfig])](#new_xmm_new)
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
    * [.getModel()](#xmm+getModel) ⇒ <code>Object</code>
    * [.getModelType()](#xmm+getModelType) ⇒ <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code>
    * [.reset()](#xmm+reset)
    * [.filter(observation)](#xmm+filter) ⇒ <code>Object</code>
    * [.getConfig([configParam])](#xmm+getConfig) ⇒ <code>[xmmModelConfig](#xmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code>
    * [.setConfig(config)](#xmm+setConfig)

<a name="new_xmm_new"></a>

### new xmm([modelType], [modelConfig])
The main xmm class.


| Param | Type | Default | Description |
| --- | --- | --- | --- |
| [modelType] | <code>&#x27;gmm&#x27;</code> &#124; <code>&#x27;hhmm&#x27;</code> | <code>&#x27;gmm&#x27;</code> | The type of model. |
| [modelConfig] | <code>[xmmModelConfig](#xmmModelConfig)</code> |  | Configuration parameters for the model. |

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

<a name="xmm+getModel"></a>

### xmm.getModel() ⇒ <code>Object</code>
Returns the trained model (the same object as in [TrainCallback](TrainCallback)).

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

### xmm.getConfig([configParam]) ⇒ <code>[xmmModelConfig](#xmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code>
Get the actual model configuration parameters or one of them.

**Kind**: instance method of <code>[xmm](#xmm)</code>  
**Returns**: <code>[xmmModelConfig](#xmmModelConfig)</code> &#124; <code>Number</code> &#124; <code>String</code> &#124; <code>Boolean</code> - Depends on the parameter.
<br /><br />If called without any argument :
<li style="list-style-type: none;">
<ul> an object of type <code>xmmModelConfig</code> containing all the actual model configuration parameters</ul>
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
| config | <code>[xmmModelConfig](#xmmModelConfig)</code> | A config object as returned by [getConfig](getConfig) when called without arguments (missing parameters won't be changed internally and invalid ones will be ignored). |

<hr>
<a name="xmmModelConfig"></a>

## xmmModelConfig
**Kind**: global typedef  
**Properties**

| Name | Type | Default | Description |
| --- | --- | --- | --- |
| gaussians | <code>Number</code> | <code>1</code> | the number of gaussians used for encoding a state. |
| relative_regularization | <code>Number</code> | <code>0.01</code> | the relative regularization (see XMM documentation). |
| absolute_regularization | <code>Number</code> | <code>0.01</code> | the absolute regularization (see XMM documentation). |
| covariance_mode | <code>&#x27;diagonal&#x27;</code> &#124; <code>&#x27;full&#x27;</code> | <code>&#x27;full&#x27;</code> | the type of covariance matrix used in the model. |
| hierarchical | <code>Boolean</code> | <code>true</code> | if <code>model</code> is 'hhmm', turns hierarchical mode on/off. |
| states | <code>Number</code> | <code>1</code> | if <code>model</code> is 'hhmm', defines the number of states used to generate each individual hmm. |
| transition_mode | <code>&#x27;ergodic&#x27;</code> &#124; <code>&#x27;leftright&#x27;</code> | <code>&#x27;leftright&#x27;</code> | if <code>model</code> is 'hhmm', sets the transition mode between the individual hmm models. |
| regression_estimator | <code>&#x27;full&#x27;</code> &#124; <code>&#x27;windowed&#x27;</code> &#124; <code>&#x27;likeliest&#x27;</code> | <code>&#x27;full&#x27;</code> | if <code>model</code> is 'hhmm', the type of estimator used for regression with hmms. |

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
