# AdvCAD Application
add_executable(advcad advcad.cc)
target_link_libraries(advcad WH)

# Link math library (required by original build)
target_link_libraries(advcad m)

# Include WH headers
target_include_directories(advcad PRIVATE ../WH)