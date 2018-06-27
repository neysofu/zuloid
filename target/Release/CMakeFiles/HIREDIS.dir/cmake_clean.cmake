file(REMOVE_RECURSE
  "../../lib/hiredis/libhiredis.a"
  "libHIREDIS.pdb"
  "libHIREDIS.dylib"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/HIREDIS.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
