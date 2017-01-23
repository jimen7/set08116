cmake_minimum_required(VERSION 2.8.2)

project(devil-download NONE)

include(ExternalProject)
ExternalProject_Add(devil-download
                    GIT_REPOSITORY;https://github.com/dooglz/DevIL.git;GIT_TAG;7129e65
                    SOURCE_DIR          "C:/Users/40204497/Desktop/set08116/labs_build/devil-src"
                    BINARY_DIR          "C:/Users/40204497/Desktop/set08116/labs_build/devil-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
)
