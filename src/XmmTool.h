#ifndef _XMM_TOOL_H_
#define _XMM_TOOL_H_

#include <nan.h>
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
  virtual void train(Nan::Callback *callback, xmm::TrainingSet *set) = 0;
  virtual void cancelTraining() = 0;
  virtual v8::Local<v8::Object> filter(std::vector<float> observation) = 0;

  virtual std::size_t getGaussians() = 0;
  virtual void setGaussians(std::size_t gaussians) = 0;
  virtual double getRelativeRegularization() = 0;
  virtual void setRelativeRegularization(double relReg) = 0;
  virtual double getAbsoluteRegularization() = 0;
  virtual void setAbsoluteRegularization(double absReg) = 0;
  virtual xmm::GaussianDistribution::CovarianceMode getCovarianceMode() = 0;
  virtual void setCovarianceMode(xmm::GaussianDistribution::CovarianceMode cm) = 0;
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
    // model.configuration.multithreading = xmm::MultithreadingMode::Sequential;

    // this allows us to cancel the training process running in the asyncworker
    model.configuration.multithreading = xmm::MultithreadingMode::Background;
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

  void train(Nan::Callback *callback, xmm::TrainingSet *set) {
    // callbacks.push_back(callback);
    workers.push_back(new XmmWrapTrainWorker<Model>(callback, model, set));
    Nan::AsyncQueueWorker(workers[workers.size() - 1]);
  }

  void cancelTraining() {
    for (auto worker : workers) {
      worker->Stop();
    }

    workers.clear();
    // callbacks.clear();
  }

  v8::Local<v8::Object> filter(std::vector<float> observation) {
    v8::Local<v8::Object> outputResults = Nan::New<v8::Object>();
  
    bool bimodal = model.shared_parameters->bimodal.get();
    unsigned int nmodels = model.size();
    unsigned int dimension = model.shared_parameters->dimension.get();
    unsigned int dimension_input = model.shared_parameters->dimension_input.get();
    unsigned int dimension_output = dimension - dimension_input;

    model.filter(observation);
    xmm::Results<ModelType> res = model.results;

    v8::Local<v8::Array> instant_likelihoods = Nan::New<v8::Array>(nmodels);
    v8::Local<v8::Array> instant_normalized_likelihoods = Nan::New<v8::Array>(nmodels);
    v8::Local<v8::Array> smoothed_likelihoods = Nan::New<v8::Array>(nmodels);
    v8::Local<v8::Array> smoothed_normalized_likelihoods = Nan::New<v8::Array>(nmodels);
    v8::Local<v8::Array> smoothed_log_likelihoods = Nan::New<v8::Array>(nmodels);

    for (std::size_t i = 0; i < nmodels; ++i) {
      Nan::Set(instant_likelihoods, i,
               Nan::New(res.instant_likelihoods[i]));
      Nan::Set(instant_normalized_likelihoods, i,
               Nan::New(res.instant_normalized_likelihoods[i]));
      Nan::Set(smoothed_likelihoods, i,
               Nan::New(res.smoothed_likelihoods[i]));
      Nan::Set(smoothed_normalized_likelihoods, i,
               Nan::New(res.smoothed_normalized_likelihoods[i]));
      Nan::Set(smoothed_log_likelihoods, i,
               Nan::New(res.smoothed_log_likelihoods[i]));
    }

    outputResults->Set(
      Nan::New<v8::String>("instant_likelihoods").ToLocalChecked(),
      instant_likelihoods
    );
    outputResults->Set(
      Nan::New<v8::String>("instant_normalized_likelihoods").ToLocalChecked(),
      instant_normalized_likelihoods
    );
    outputResults->Set(
      Nan::New<v8::String>("smoothed_likelihoods").ToLocalChecked(),
      smoothed_likelihoods
    );
    outputResults->Set(
      Nan::New<v8::String>("smoothed_normalized_likelihoods").ToLocalChecked(),
      smoothed_normalized_likelihoods
    );
    outputResults->Set(
      Nan::New<v8::String>("smoothed_log_likelihoods").ToLocalChecked(),
      smoothed_log_likelihoods
    );

    outputResults->Set(
      Nan::New<v8::String>("likeliest").ToLocalChecked(),
      Nan::New<v8::String>(res.likeliest).ToLocalChecked()
    );

    if (bimodal) {
      v8::Local<v8::Array> output_values = Nan::New<v8::Array>(dimension_output);
      for (unsigned int i = 0; i < dimension_output; ++i) { 
        Nan::Set(output_values, i, Nan::New(res.output_values[i]));
      }
      outputResults->Set(
        Nan::New<v8::String>("output_values").ToLocalChecked(),
        output_values
      );

      unsigned int dim_out_cov = res.output_covariance.size();
      v8::Local<v8::Array> output_covariance = Nan::New<v8::Array>(dim_out_cov);
      for (unsigned int i = 0; i < dim_out_cov; ++i) { 
        Nan::Set(output_covariance, i, Nan::New(res.output_covariance[i]));
      }
      outputResults->Set(
        Nan::New<v8::String>("output_covariance").ToLocalChecked(),
        output_covariance
      );
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

};

#endif /* _XMM_TOOL_H_ */
