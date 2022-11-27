cmake_minimum_required (VERSION 3.8)

project ("hamArc")

add_executable (hamArc "bin/main.cpp"  "lib/ham.cpp" "lib/ham.h" "lib/hamDecode.cpp"
"lib/hamDecode.h" "lib/hamEncode.cpp" "lib/hamEncode.h" "lib/arc.cpp" "lib/arc.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hamArc PROPERTY CXX_STANDARD 20)
endif()
