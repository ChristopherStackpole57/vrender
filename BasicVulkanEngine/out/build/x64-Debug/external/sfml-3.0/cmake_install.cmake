# Install script for directory: C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/external/sfml-3.0

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/src/SFML/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/external/sfml-3.0/include/" FILES_MATCHING REGEX "/[^/]*\\.hpp$" REGEX "/[^/]*\\.inl$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE DIRECTORY FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/external/sfml-3.0/cmake/Modules/")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE DIRECTORY FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/lib/" FILES_MATCHING REGEX "/[^/]*\\.pdb$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/external/sfml-3.0/license.md")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/external/sfml-3.0/readme.md")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLSystemStaticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLSystemStaticTargets.cmake"
         "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLSystemStaticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLSystemStaticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLSystemStaticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLSystemStaticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLSystemStaticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLMainStaticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLMainStaticTargets.cmake"
         "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLMainStaticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLMainStaticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLMainStaticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLMainStaticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLMainStaticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLWindowStaticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLWindowStaticTargets.cmake"
         "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLWindowStaticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLWindowStaticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLWindowStaticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLWindowStaticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLWindowStaticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLNetworkStaticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLNetworkStaticTargets.cmake"
         "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLNetworkStaticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLNetworkStaticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLNetworkStaticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLNetworkStaticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLNetworkStaticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLGraphicsStaticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLGraphicsStaticTargets.cmake"
         "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLGraphicsStaticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLGraphicsStaticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLGraphicsStaticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLGraphicsStaticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLGraphicsStaticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLAudioStaticTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLAudioStaticTargets.cmake"
         "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLAudioStaticTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLAudioStaticTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML/SFMLAudioStaticTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLAudioStaticTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/CMakeFiles/Export/3937c6824958577f216dad0a66bc6149/SFMLAudioStaticTargets-debug.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SFML" TYPE FILE FILES
    "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/SFMLConfig.cmake"
    "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/SFMLConfigVersion.cmake"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/external/sfml-3.0/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
