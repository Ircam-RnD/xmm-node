#ifndef _XMM_TOOL_BASE_H_
#define _XMM_TOOL_BASE_H_

#include "../xmm/src/xmm.h"

// our common interface class for polymorphism :
// TODO : change methods to match those of XmmWrapUtils.h

// class XmmToolWrap {

// };

class XmmToolBase {
public:
	virtual void addToTrainingSet(std::vector<std::string> phrases) = 0;
	virtual void clearTrainingSet() = 0;
	virtual void train(xmm::TrainingSet *s) = 0;
	virtual void filter(std::vector<float> const& observation);
	virtual bool training() = 0;
	virtual Json::Value toJson() = 0;
	virtual std::string toString() = 0;
	virtual void setNbGaussians(std::size_t n) = 0;
	virtual void setRelativeRegularization(double relReg) = 0;
	virtual void setAbsoluteRegularization(double absReg) = 0;
};

// the specializable template :
// here manage the index list to ease phrases management (as in unity binding)

template<typename Model>
class XmmTool : public XmmToolBase {

public:
	xmm::TrainingSet set;
	Model model;

	XmmTool() {
			model.configuration.multithreading = xmm::MultithreadingMode::Sequential;
	}

	~XmmTool() {}
	
	virtual void addToTrainingSet(std::vector<std::string> phrases) {
		xmm::Phrase xp;
		Json::Value jp;
		Json::Reader jr;
		for (std::size_t i=0; i<phrases.size(); i++) {
				
			//std::cout << phrases[i] << std::endl;
			if (jr.parse(phrases[i], jp)) {
				xp.fromJson(jp);
			} else {
				throw std::runtime_error("Cannot Parse Json String");
			}
			//xp.fromString(phrases[i]);
			/*
			Json::Value jp2 = xp.toJson();
			Json::StyledWriter w;
			std::cout << w.write(jp2) << std::endl;
			//*/
			
			// first set dimension and column names otherwise they will be reinitialized by "onAttributeChange"
			if (i==0) {
				set.dimension.set(xp.dimension.get());
				set.column_names.set(xp.column_names, true);
			}
			set.addPhrase(static_cast<int>(set.size()), xp);
		}
		//set.dimension.set(xp.dimension.get());
		//std::cout << xp.column_names[0] << std::endl;
		//set.column_names.set(xp.column_names, true);
		/*
		Json::Value jset = set.toJson();
		Json::StyledWriter w;
		std::cout << w.write(jset) << std::endl;
		//*/        
	}

	virtual void addPhrase(xmm::Phrase xp) {

	}
	
	virtual void clearTrainingSet() {
		set.clear();
	}
	
	virtual void train(xmm::TrainingSet *s) {
		//model.train(&s);
		model.train(s);
	}

	virtual bool training() {
		return model.training();
	}
	
	virtual void filter(std::vector<float> const& observation) {
		model.filter(observation);
	}

	virtual Json::Value toJson() {
		return model.toJson();        
	}
	
	virtual std::string toString() {
		Json::Value jmodels = model.toJson();
		Json::FastWriter fw;
		return fw.write(jmodels);
	}
	
	// configuration
	
	virtual void setNbGaussians(std::size_t n) {
		if (n > 0) {
			model.configuration.gaussians.set(n, 1);
			model.configuration.changed = true;
		}
		//std::cout << gmm.configuration.gaussians.get() << std::endl;
	}
	
	virtual void setRelativeRegularization(double relReg) {
		if (relReg > 0) {
			model.configuration.relative_regularization.set(relReg);
			model.configuration.changed = true;
		}
	}

	virtual void setAbsoluteRegularization(double absReg) {
		if (absReg > 0) {
				model.configuration.absolute_regularization.set(absReg);
				model.configuration.changed = true;
		}
	}
};


#endif /* _XMM_TOOL_BASE_H_ */
