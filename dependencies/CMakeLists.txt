# ----- DEPENDENCIES BUILD CONFIG -----
# Many thanks to
# https://eliasdaler.github.io/using-cmake/
include(FetchContent)

FetchContent_Declare(
    qt5
    URL "https://github.com/qt/qtbase/archive/refs/tags/v5.12.12.zip"
    URL_HASH MD5=d7124c382965565e2daa9df0cfdfbf3a # md5sum
)

#Currently cmake doesn't allow you to use the downloaded version of pugixml
#and insists on using the globally-installed version
#TODO get this pugixml bug fixed
#FetchContent_Declare(
#    pugixml
#    URL "https://github.com/zeux/pugixml/releases/download/v1.12/pugixml-1.12.zip"
#    URL_HASH MD5=48b37d7201c02762c84aff14372be90f # also md5sum
#)

FetchContent_Declare(
    resvg
    URL "https://github.com/RazrFalcon/resvg/archive/refs/tags/v0.21.0.zip"
)

add_subdirectory(qt5)
add_subdirectory(resvg)
#add_subdirectory(pugixml)
