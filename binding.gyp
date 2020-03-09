{
  "targets": [
    {
      "target_name": "xmm",
      # "cflags!": [ "-fno-exceptions" ],
      # "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
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
        "xmm/dependencies/jsoncpp/src/json_writer.cpp",
        "src/JsonCppNapiConverters.cpp",
        "src/XmmWrap.cpp",
        "src/xmm.cpp"
      ],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      #'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'defines': [ 'NAPI_ENABLE_CPP_EXCEPTIONS' ],

      "cflags" : ["-w", "-std=c++11", "-fexceptions"],
      # "cflags": ["-Wall", "-std=c++11", "-fexceptions"],
      "cflags_cc" : ["-w", "-std=c++11", "-fexceptions"],
      #"cflags_cc" : ["-Wall", "-std=c++11", "-fexceptions"],
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
    },
    {
      "target_name" : "action_after_build",
      "type" : "none",
      "dependencies" : [ "<(module_name)" ],
      "copies" : [
        {
          "files" : [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination" : "<(module_path)"
        }
      ]
    }
  ]
}