add_subdirectory(assets)

if(EMSCRIPTEN)
  # Copy the index.html file.
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/index.html
    $ENV{website}/index.html
    COPYONLY
  )
endif()

function(add_src target input)
  set(ns_target smk_apps_${target})
  add_executable(${ns_target} ${input})
  set_target_properties(${ns_target} PROPERTIES OUTPUT_NAME ${target})
  target_link_libraries(${ns_target} PRIVATE smk smk_apps_asset)
  set_property(TARGET ${ns_target} PROPERTY CXX_STANDARD 17)

  # Detect emscripten is used.
  if(EMSCRIPTEN)
    # Release flags
    set_property(TARGET ${ns_target} APPEND_STRING PROPERTY LINK_FLAGS
      " -sALLOW_MEMORY_GROWTH"
      " -sDEMANGLE_SUPPORT"
      " -pthread"
      " -s EXPORT_NAME='hmi' -s MODULARIZE=1"
    )
    set_property(GLOBAL APPEND_STRING PROPERTY LINK_FLAGS
      " -s WASM=1"
      " --js-opts 3"
      " --llvm-lto 3"
      " --llvm-opts 3"
      " -O3"
    )

    # 添加链接标志
    target_link_libraries(${ns_target} PUBLIC "-s USE_GLFW=3")
    target_link_libraries(${ns_target} PUBLIC "-s USE_WEBGL2=1")
    target_link_libraries(${ns_target} PUBLIC "-s FULL_ES2=1")
    target_link_libraries(${ns_target} PUBLIC "-s FULL_ES3=1")
    target_link_libraries(${ns_target} PUBLIC "-s USE_SDL=2")
    target_link_libraries(${ns_target} PUBLIC "-s WASM_WORKERS=1")
    target_link_libraries(${ns_target} PUBLIC "-s USE_PTHREADS")
    target_link_libraries(${ns_target} PUBLIC "-s USE_SQLITE3=1")
    target_link_libraries(${ns_target} PUBLIC "-s FORCE_FILESYSTEM")
    target_link_libraries(${ns_target} PUBLIC "-s FETCH=1")
    target_link_libraries(${ns_target} PUBLIC "-l embind")

    target_link_libraries(${ns_target} PUBLIC "-s USE_ZLIB=1")
    target_link_libraries(${ns_target} PUBLIC "-l idbfs.js")
    target_link_libraries(${ns_target} PUBLIC "-l websocket.js")
    target_link_libraries(${ns_target} PUBLIC "-s USE_BOOST_HEADERS=1")
    target_link_libraries(${ns_target} PUBLIC "-s EXPORTED_FUNCTIONS='_main,_malloc,_free'")
    target_link_libraries(${ns_target} PUBLIC "-s EXPORTED_RUNTIME_METHODS='ccall,cwrap,UTF8ToString'")
    target_link_libraries(${ns_target} PUBLIC "-s MODULARIZE=1")

    # Allow some files to be fetched.
    file(GLOB_RECURSE files "./assets/*")

    foreach(file ${files})
      file(RELATIVE_PATH relative_file ${CMAKE_SOURCE_DIR} "${file}")
      set_property(TARGET ${ns_target} APPEND_STRING PROPERTY LINK_FLAGS " --preload-file \"${file}@/${relative_file}\"")
    endforeach()
  endif()
endfunction(add_src)

# app source
file(GLOB app_src
  ${CMAKE_SOURCE_DIR}/apps/*.cpp
)
message(STATUS "app_src is: ${app_src}")

# 编译执行
add_src(hmi ${app_src})
