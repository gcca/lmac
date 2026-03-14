cmake_minimum_required(VERSION 4.2.3)
project(lmac CXX)

find_package(ftxui REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

add_library(lmaclib OBJECT
  commands/Claude/ChooseAction.cpp
  commands/ClaudeCLI/ChooseAction.cpp
  commands/ClaudeCLI/Entry.cpp
  commands/ClaudeCLI/ListAction.cpp
  commands/ClaudeCLI/ShowCredsAction.cpp
  commands/ClaudeCLICommand.cpp
  commands/ClaudeCommand.cpp
  commands/ClaudeDesk/ChooseAction.cpp
  commands/ClaudeDesk/Installation.cpp
  commands/ClaudeDesk/ListAction.cpp
  commands/ClaudeDeskCommand.cpp
  commands/Gh/Search/IssuesAction.cpp
  commands/Gh/Search/PrsAction.cpp
  commands/Gh/SearchCommand.cpp
  commands/Gh/Sm/IssueAction.cpp
  commands/Gh/Sm/PrAction.cpp
  commands/Gh/SmCommand.cpp
  commands/GhCommand.cpp
  commands/HelpCommand.cpp
  commands/Shell/FormatAction.cpp
  commands/ShellCommand.cpp
)

target_include_directories(lmaclib PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(lmaclib
  PUBLIC
    Boost::program_options
    ftxui::component
    "-framework CoreFoundation"
    "-framework Security"
)

add_executable(lmac lmac.cc)

target_link_libraries(lmac PRIVATE lmaclib)

install(TARGETS lmac
  RUNTIME DESTINATION $ENV{HOME}/.local/bin
)

option(LMAC_TEST "Build tests" OFF)

if(LMAC_TEST)
  find_package(GTest REQUIRED)

  add_executable(lmac_test
    commands/Claude/ChooseAction-test.cc
    commands/ClaudeCLI/ChooseAction-test.cc
    commands/ClaudeCLI/ListAction-test.cc
    commands/ClaudeCLI/ShowCredsAction-test.cc
    commands/ClaudeCLICommand-test.cc
    commands/ClaudeCommand-test.cc
    commands/ClaudeDesk/ChooseAction-test.cc
    commands/ClaudeDesk/ListAction-test.cc
    commands/ClaudeDeskCommand-test.cc
    commands/Gh/Search/IssuesAction-test.cc
    commands/Gh/Search/PrsAction-test.cc
    commands/Gh/SearchCommand-test.cc
    commands/Gh/Sm/IssueAction-test.cc
    commands/Gh/Sm/PrAction-test.cc
    commands/Gh/SmCommand-test.cc
    commands/GhCommand-test.cc
    commands/HelpCommand-test.cc
    commands/Shell/FormatAction-test.cc
    commands/ShellCommand-test.cc
  )

  target_link_libraries(lmac_test
    PRIVATE
      lmaclib
      GTest::gtest_main
      GTest::gmock
  )

  include(GoogleTest)
  gtest_discover_tests(lmac_test)
endif()
