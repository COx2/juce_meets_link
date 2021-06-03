juce_add_gui_app(JuceLink
    # VERSION ...                       # Set this if the app version is different to the project version
    # ICON_BIG ...                      # ICON_* arguments specify a path to an image file to use as an icon
    # ICON_SMALL ...
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "Juce Link")     # The name of the final executable, which can differ from the target name

juce_generate_juce_header(JuceLink)

target_sources(JuceLink
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Source/Main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/MainComponent.cpp
)

target_compile_definitions(JuceLink
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:JuceLink,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:JuceLink,JUCE_VERSION>"
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_compile_features(JuceLink
    PRIVATE 
        cxx_std_17
)

target_compile_options(JuceLink
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror=return-type>
)

# Link with Ableton::Link library via CMake.
include(${CMAKE_CURRENT_LIST_DIR}/../../External/link/AbletonLinkConfig.cmake)
target_link_libraries(JuceLink
    PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_utils
        Ableton::Link
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
