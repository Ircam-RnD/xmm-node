#ifndef _XMM_TOOL_H_
#define _XMM_TOOL_H_

#include "../xmm/src/xmm.h"
#include "XmmWrapTrainWorker.h"

// our common interface class for polymorphism :
// TODO : change methods to match those of XmmWrapUtils.h

class XmmToolBase {
public:
  virtual void setBimodal(bool multimodality) = 0;
  virtual Json::Value getModel() = 0;
  virtual void setModel(Json::Value jm) = 0;
  virtual void reset() = 0;
  virtual void train(Napi::Function& callback, xmm::TrainingSet *set) = 0;
  virtual void cancelTraining() = 0;
  virtual Json::Value filter(std::vector<float> observation) = 0;

  virtual std::size_t getGaussians() = 0;
  virtual void setGaussians(std::size_t gaussians) = 0;
  virtual double getRelativeRegularization() = 0;
  virtual void setRelativeRegularization(double relReg) = 0;
  virtual double getAbsoluteRegularization() = 0;
  virtual void setAbsoluteRegularization(double absReg) = 0;
  virtual xmm::GaussianDistribution::CovarianceMode getCovarianceMode() = 0;
  virtual void setCovarianceMode(xmm::GaussianDistribution::CovarianceMode cm) = 0;
  virtual xmm::MultiClassRegressionEstimator getMultiClassRegressionEstimator() = 0;
  virtual void setMultiClassRegressionEstimator(xmm::MultiClassRegressionEstimator mre) = 0;
};

// the specializable template :
// here manage the index list to ease phrases management (as in unity binding)

template<typename Model, typename ModelType>
class XmmTool : public XmmToolBase {
private:
  std::vector<XmmWrapTrainWorker<Model> *> workers;
  // std::vector<Nan::Callback *> callbacks;

public:
  Model model;

  XmmTool(bool bimodal = false) {
    model = Model(bimodal);
    model.configuration.multithreading = xmm::MultithreadingMode::Sequential;

    // this allows us to cancel the training process running in the asyncworker
    // model.configuration.multithreading = xmm::MultithreadingMode::Background;
  }

  ~XmmTool() {}

  void setBimodal(bool multimodality) {
    // Model tmp = Model(model);
    xmm::Configuration<ModelType> config = model.configuration;
    model = Model(multimodality);
    model.configuration = config;
  }

  Json::Value getModel() {
    return model.toJson();
  }

  void setModel(Json::Value jm) {
    model.fromJson(jm);
  }

  void reset() {
    model.reset();
  }

  void train(Napi::Function& callback, xmm::TrainingSet *set) {
    // callbacks.push_back(callback); // no need for callbacks

    /*
    for (int i = workers.size() - 1; i >= 0; i--) {
      if (workers[i]->Done()) {
        // No need to delete the pointer as it's probably freed after call to HandleOKCallback
        // We can deduce this from the fact that we're not supposed to call delete
        // if (workers[i] != nullptr) delete workers[i];
        workers.erase(workers.begin() + i);
      }
    }
    //*/

    // Already tried to replace new by make_shared an declare workers as vector of shared_ptrs
    // But this crashes, probably because of some automatic call to delete

    workers.push_back(new XmmWrapTrainWorker<Model>(callback, model, set));
    workers[workers.size() - 1]->Queue();
    // Nan::AsyncQueueWorker(workers[workers.size() - 1]);

    // Nan::AsyncQueueWorker(new XmmWrapTrainWorker<Model>(callback, model, set));

    // XmmWrapTrainWorker<Model>* worker = new XmmWrapTrainWorker<Model>(callback, model, set);
    // worker->Queue();
  }

  void cancelTraining() {
    for (auto worker : workers) {
      worker->Stop();
    }

    workers.clear();

    // callbacks.clear(); // no need for callbacks
  }

  // TODO : store a Json::Value of outputResults as a variable of the class
  // to avoid creating it on each call to filter() and only fill it instead
  // (e.g. add an "updateFromModel" method or something)
  Json::Value filter(std::vector<float> observation) {
    Json::Value outputResults;
    // return outputResults;

    bool bimodal = model.shared_parameters->bimodal.get();
    unsigned int nmodels = model.size();
    unsigned int dimension = model.shared_parameters->dimension.get();
    unsigned int dimension_input = model.shared_parameters->dimension_input.get();
    unsigned int dimension_output = dimension - dimension_input;

    model.filter(observation);
    xmm::Results<ModelType> res = model.results;

    Json::Value instant_likelihoods;
    instant_likelihoods.resize(static_cast<Json::ArrayIndex>(nmodels));

    Json::Value instant_normalized_likelihoods;
    instant_normalized_likelihoods.resize(static_cast<Json::ArrayIndex>(nmodels));

    Json::Value smoothed_likelihoods;
    smoothed_likelihoods.resize(static_cast<Json::ArrayIndex>(nmodels));

    Json::Value smoothed_normalized_likelihoods;
    smoothed_normalized_likelihoods.resize(static_cast<Json::ArrayIndex>(nmodels));

    Json::Value smoothed_log_likelihoods;
    smoothed_log_likelihoods.resize(static_cast<Json::ArrayIndex>(nmodels));

    for (int i = 0; i < nmodels; i++) {
      // instant_likelihoods[i] = Json::Value(res.instant_likelihoods[i]);
      // instant_normalized_likelihoods[i] = Json::Value(res.instant_normalized_likelihoods[i]);
      // smoothed_likelihoods[i] = Json::Value(res.smoothed_likelihoods[i]);
      // smoothed_normalized_likelihoods[i] = Json::Value(res.smoothed_normalized_likelihoods[i]);
      // smoothed_log_likelihoods[i] = Json::Value(res.smoothed_log_likelihoods[i]);
      instant_likelihoods[i] = res.instant_likelihoods[i];
      instant_normalized_likelihoods[i] = res.instant_normalized_likelihoods[i];
      smoothed_likelihoods[i] = res.smoothed_likelihoods[i];
      smoothed_normalized_likelihoods[i] = res.smoothed_normalized_likelihoods[i];
      smoothed_log_likelihoods[i] = res.smoothed_log_likelihoods[i];
    }

    outputResults["instant_likelihoods"] = instant_likelihoods;
    outputResults["instant_normalized_likelihoods"] = instant_normalized_likelihoods;
    outputResults["smoothed_likelihoods"] = smoothed_likelihoods;
    outputResults["smoothed_normalized_likelihoods"] = smoothed_normalized_likelihoods;
    outputResults["smoothed_log_likelihoods"] = smoothed_log_likelihoods;
    outputResults["likeliest"] = res.likeliest; // check res.likeliest is a std::string

    if (bimodal) {
      Json::Value outputValues;
      outputValues.resize(static_cast<Json::ArrayIndex>(dimension_output));
      
      unsigned int dim_out_cov = res.output_covariance.size();
      Json::Value outputCovariance;
      outputCovariance.resize(static_cast<Json::ArrayIndex>(dim_out_cov));

      for (unsigned int i = 0; i < dimension_output; i++) {
        outputValues[i] = res.output_values[i]; // Json::Value(res.output_values[i]);
      }
      
      for (unsigned int i = 0; i < dim_out_cov; ++i) {
        outputCovariance[i] = res.output_covariance[i]; // Json::Value(res.output_covariance[i]);
      }

      outputResults["output_values"] = outputValues;
      outputResults["output_covariance"] = outputCovariance;
    }

    return outputResults;
  }

  //========================= GETTERS / SETTERS ==============================//

  std::size_t getGaussians() {
    return model.configuration.gaussians.get();
  }

  void setGaussians(std::size_t gaussians) {
    model.configuration.gaussians.set(gaussians);
    model.configuration.changed = true;
  }

  double getRelativeRegularization() {
    return model.configuration.relative_regularization.get();
  }

  void setRelativeRegularization(double relReg) {
    model.configuration.relative_regularization.set(relReg);
    model.configuration.changed = true;
  }

  double getAbsoluteRegularization() {
    return model.configuration.absolute_regularization.get();
  }

  void setAbsoluteRegularization(double absReg) {
    model.configuration.absolute_regularization.set(absReg);
    model.configuration.changed = true;
  }

  xmm::GaussianDistribution::CovarianceMode getCovarianceMode() {
    return model.configuration.covariance_mode.get();
  }

  void setCovarianceMode(xmm::GaussianDistribution::CovarianceMode cm) {
    model.configuration.covariance_mode.set(cm);
    model.configuration.changed = true;
  }

  xmm::MultiClassRegressionEstimator getMultiClassRegressionEstimator() {
    return model.configuration.multiClass_regression_estimator;
  }

  void setMultiClassRegressionEstimator(xmm::MultiClassRegressionEstimator mre) {
    model.configuration.multiClass_regression_estimator = mre;
    model.configuration.changed = true;
  }
};

#endif /* _XMM_TOOL_H_ */
