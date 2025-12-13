cmake_minimum_required(VERSION 3.20)
project(CLIC C)

# cJSON importieren (vcpkg muss eingebunden sein)
find_package(cJSON CONFIG REQUIRED)
find_package(CURL REQUIRED)

# Executable erstellen
add_executable(CLIC main.c game.c)

# Bibliotheken verlinken
target_link_libraries(CLIC PRIVATE cjson cjson_utils)
target_link_libraries(CLIC PRIVATE CURL::libcurl)