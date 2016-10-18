{
	"targets": [
		{
			"target_name" : "xmm",
			"sources" : [

				"src/xmm.cpp",
				"src/XmmWrap.cpp",
				# "src/XmmTool.cpp",
				"src/JsonCppV8Converters.cpp",

				# "src-wrapped-objects/xmm.cpp",
				# "src-wrapped-objects/phrase.cpp",
				# "src-wrapped-objects/trainingset.cpp",

				"xmm/src/core/common/xmmAttribute.cpp",
				"xmm/src/core/common/xmmJson.cpp",
				"xmm/src/core/distributions/xmmGaussianDistribution.cpp",
				"xmm/src/core/model/xmmModelSharedParameters.cpp",
				"xmm/src/core/model/xmmModelSingleClass.cpp",
				"xmm/src/core/trainingset/xmmPhrase.cpp",
				"xmm/src/core/trainingset/xmmTrainingSet.cpp",
				"xmm/src/models/gmm/xmmGmm.cpp",
				"xmm/src/models/gmm/xmmGmmParameters.cpp",
				"xmm/src/models/gmm/xmmGmmSingleClass.cpp",
				"xmm/src/models/hmm/xmmHierarchicalHmm.cpp",
				"xmm/src/models/hmm/xmmHmmParameters.cpp",
				"xmm/src/models/hmm/xmmHmmSingleClass.cpp",
				"xmm/src/models/kmeans/xmmKMeans.cpp",
				"xmm/src/models/kmeans/xmmKMeansParameters.cpp",
				"xmm/dependencies/jsoncpp/src/json_reader.cpp",
				"xmm/dependencies/jsoncpp/src/json_value.cpp",
				"xmm/dependencies/jsoncpp/src/json_writer.cpp"
			],
			"include_dirs" : [
				"<!(node -e \"require('nan')\")",
				#"xmm/src/xmm.h"
			],

			#"cflags": ["-Wall", "-std=c++11"],
			"cflags" : ["-std=c++11", "-fexceptions"],
			"cflags_cc" : ["-std=c++11", "-fexceptions"],
			#"cflags!" : [ '-fno-exceptions' ],
			#"cflags_cc!" : [ '-fno-exceptions' ],
			"conditions" : [
				[ 'OS=="mac"', {
				    	"xcode_settings" : {
				        	'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
				        	'OTHER_LDFLAGS': ['-stdlib=libc++'],
				        	'MACOSX_DEPLOYMENT_TARGET': '10.7',
				        	'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
				        	'WARNING_CFLAGS' : ['-w'] #silence all warnings
						}
				}]
			] 
		}
	]
}
