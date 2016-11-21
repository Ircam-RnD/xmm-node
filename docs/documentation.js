/**
 * @typedef xmmModelConfig
 * @property {Number} [gaussians=1] - the number of gaussians used for encoding a state.
 * @property {Number} [relativeRegularization=0.01] - the relative regularization (see XMM documentation).
 * @property {Number} [absoluteRegularization=0.01] - the absolute regularization (see XMM documentation).
 * @property {('diagonal'|'full')} [covarianceMode='full'] - the type of covariance matrix used in the model.
 * @property {Boolean} [hierarchical=true] - if <code>model</code> is 'hhmm', turns hierarchical
 * mode on/off.
 * @property {Number} [states=1] - if <code>model</code> is 'hhmm', defines the number of states
 * used to generate each individual hmm.
 * @property {('ergodic'|'leftright')} [transitionMode='leftright'] - if <code>model</code> is 'hhmm',
 * sets the transition mode between the states of the individual hmm models.
 * @property {('full'|'windowed'|'likeliest')} [regressionEstimator='full'] - if <code>model</code> is 'hhmm',
 * the type of estimator used for regression with hmms.
 */

/**
 * The main xmm class.
 * @name xmm
 * @class
 * @param {('gmm'|'hhmm')} [modelType='gmm'] - The type of model.
 * @param {xmmModelConfig} [modelConfig] - Configuration parameters for the model.
 */

//============================== configuration ===============================//

/**
 * Get the actual model configuration parameters or one of them.
 * @method getConfig
 * @memberof xmm
 * @instance
 * @param {('gaussians'|'relativeRegularization'|'absoluteRegularization'|
 *	'covarianceMode'|'hierarchical'|'states'|'transitionMode'|'regressionEstimator')}
 * [configParam] - The name of a configuration parameter.
 * @returns {(xmmModelConfig|Number|String|Boolean)} Depends on the parameter.
 * <br /><br />If called without any argument, returns :
 * <li style="list-style-type: none;">
 * <ul> an object of type <code>xmmModelConfig</code> containing all the actual model configuration parameters</ul>
 * </li>
 * Otherwise, the returned value type depends on the requested configuration parameter :
 * <li style="list-style-type: none;">
 * <ul><code>'gaussians'</code> : the number of gaussians</ul>
 * <ul><code>'relativeRegularization'</code> : the relative regularization foat value</ul>
 * <ul><code>'absoluteRegularization'</code> : the absolute regularization float value</ul>
 * <ul><code>'covarianceMode'</code> : the actual covariance mode ('full' or 'diagonal')</ul>
 * <ul><code>'hierarchical'</code> : if the model type is not 'hhmm', <code>undefined</code>,
 * otherwise <code>true</code> if the model is hierarchical and <code>false</code> if it is not</ul>
 * <ul><code>'states'</code> : if the model type is not 'hhmm', <code>undefined</code>, otherwise
 *	the number of states of the hmms</ul>
 * <ul><code>'transitionMode'</code> : if the model type is not 'hhmm', <code>undefined</code>,
 * otherwise the actual transition mode value ('ergodic' or 'leftright')</ul>
 * <ul><code>'regressionEstimator'</code> : if the model type is not 'hhmm',
 * <code>undefined</code>, otherwise the actual regression estimator used with hmms
 * ('full', 'windowed' or 'likeliest')</ul>
 * </li>
 */

/**
 * Set the actual model configuration parameters.
 * @method setConfig
 * @memberof xmm
 * @instance
 * @param {xmmModelConfig} config - A config object as returned by {@link getConfig}
 * when called without arguments (missing parameters won't be changed internally
 * and invalid ones will be ignored).
 */

//======================== phrases & training sets ===========================//

/**
 * Add a phrase to the internal training set.
 * @method addPhrase
 * @memberof xmm
 * @instance
 * @param {Object} phrase - An object containing a valid XMM phrase.
 */

/**
 * Get phrase of a certain index from the internal training set.
 * @method getPhrase
 * @memberof xmm
 * @instance
 * @param {Number} index - The index of a phrase in the training set.
 * @returns {Object} A valid XMM phrase from the internal training set.
 */

/**
 * Get phrases of a certain label from the internal training set.
 * @method getPhrasesOfLabel
 * @memberof xmm
 * @instance
 * @param {String} label - The label of which we want to get the phrases.
 * @returns {Object} A valid XMM training set containing all the requested phrases.
 */

/**
 * Remove phrase of a certain index from the internal training set.
 * @method removePhrase
 * @memberof xmm
 * @instance
 * @param {Number} index - The index of a phrase in the training set.
 */

/**
 * Remove phrases of a certain label from the internal training set.
 * @method removePhrasesOfLabel
 * @memberof xmm
 * @instance
 * @param {String} label - The label of which we want to remove the phrases.
 */

/**
 * Get the number of phrases in the training set.
 * @method getTrainingSetSize
 * @memberof xmm
 * @instance
 * @returns {Number} The number of phrases in the training set.
 */

/**
 * Get the array of all the labels in the training set.
 * @method getTrainingSetLabels
 * @memberof xmm
 * @instance
 * @returns {Array.String} An array containing all the training set's labels.
 */

/**
 * Get the actual training set as an object.
 * @method getTrainingSet
 * @memberof xmm
 * @instance
 * @returns {Object} An object containing a valid XMM training set.
 */

/**
 * Sets the actual training set.
 * @method setTrainingSet
 * @memberof xmm
 * @instance
 * @param {Object} trainingSet - An object containing a valid XMM training set.
 */

/**
 * Adds a training set to the actual training set.
 * @method addTrainingSet
 * @memberof xmm
 * @instance
 * @param {Object} trainingSet - An object containing a valid XMM training set.
 */

/**
 * Clears the training set.
 * @method clearTrainingSet
 * @memberof xmm
 * @instance
 */

//================================ training ==================================//

/**
 * Callback handling the trained model.
 * @callback trainCallback
 * @param {String} err - Description of a potential error.
 * @param {Object} res - An object containing the trained model.
 */

/**
 * Trains the model with the current training set.
 * @method train
 * @memberof xmm
 * @instance
 * @param {trainCallback} callback - The callback handling the trained model.
 */

/**
 * Cancel the current training process. <br />
 * <i><strong><font color="#f00">WARNING</font></strong>
 * This feature is experimental and may cause crashes</i>
 * @method cancelTraining
 * @memberof xmm
 * @instance
 */

/**
 * Returns the trained model (the same object as in {@link trainCallback}).
 * @method getModel
 * @memberof xmm
 * @instance
 * @returns {Object} An object containing the trained model.
 */

/**
 * Sets the actual model from another already trained model.
 * @method setModel
 * @memberof xmm
 * @instance
 * @param {Object} model - A valid XMM model of the instance's actual type.
 */

/**
 * Returns the type of the actual model.
 * @method getModelType
 * @memberof xmm
 * @instance
 * @returns {('gmm'|'hhmm')} The type of model as String.
 */

/***
 * Sets the type of the actual model.
 * @method setModelType
 * @memberof xmm
 * @instance
 * @param {String} modelType - the new model type to switch to.
 */

//================================ filtering =================================//

/**
 * Resets the internal variables used for filtering.
 * @method reset
 * @memberof xmm
 * @instance
 */

/**
 * Estimates an input array of floats.
 * @method filter
 * @memberof xmm
 * @instance
 * @param {Array.Number} observation - The observation we want an estimation of.
 * @returns {Object} filteringResults - An object containing the estimation results.
 */


