# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-src")
  file(MAKE_DIRECTORY "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-src")
endif()
file(MAKE_DIRECTORY
  "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-build"
  "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix"
  "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix/tmp"
  "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp"
  "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix/src"
  "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Programming/Projects/Cpp/VS22/LinkedIn/BasicVulkanEngine/linkedin_daily-vulkan/BasicVulkanEngine/out/build/x64-Debug/_deps/vorbis-subbuild/vorbis-populate-prefix/src/vorbis-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
