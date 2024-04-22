idf_component_register(SRCS "app_main.c"
                    INCLUDE_DIRS "."
                    REQUIRES hcf_adc
                    REQUIRES ioplaca
                    REQUIRES lcdvia595
                    )
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common
    )

