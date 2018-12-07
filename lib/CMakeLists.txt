
#add_library(SimpleSignal INTERFACE)
#target_include_directories(SimpleSignal INTERFACE include)

add_library(duktape STATIC duktape/duk_config.h duktape/duktape.c duktape/duktape.h)
target_include_directories(duktape PUBLIC duktape)

add_subdirectory(dukglue)

#--------------
