cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(rix)

find_package(Threads REQUIRED)

if ( MBOT )
    set(RIX_IPC_LIB ${CMAKE_SOURCE_DIR}/lib/mbot/librixipc.a)
    set(RIX_UTIL_LIB ${CMAKE_SOURCE_DIR}/lib/mbot/librixutil.a)
    add_compile_definitions(MBOT)
elseif ( CAEN )
    set(RIX_IPC_LIB ${CMAKE_SOURCE_DIR}/lib/caen/librixipc.a)
    set(RIX_UTIL_LIB ${CMAKE_SOURCE_DIR}/lib/caen/librixutil.a)
else()
    message(FATAL_ERROR "You must compile on either CAEN or the MBot. Please specify which OS you are using
                         If you are on the MBot: cmake -DMBOT=ON ..
                         If you are on CAEN:     cmake -DCAEN=ON ..")
endif()

if(APPLE)
    set(OS_DIR_NAME "macOS")
    add_compile_definitions(_MACOS)
elseif(UNIX)
    set(OS_DIR_NAME "linux")
elseif(WIN32)
    set(OS_DIR_NAME "win32")
    add_compile_definitions(_WIN32)
endif()

set(RPLIDAR_SDK_PATH "./rplidar_sdk/sdk/")
file(GLOB RPLIDAR_SDK_SRC "${RPLIDAR_SDK_PATH}/src/arch/${OS_DIR_NAME}/*.cpp"
                          "${RPLIDAR_SDK_PATH}/src/dataunpacker/*.cpp"
                          "${RPLIDAR_SDK_PATH}/src/dataunpacker/unpacker/*.cpp"
                          "${RPLIDAR_SDK_PATH}/src/hal/*.cpp"
                          "${RPLIDAR_SDK_PATH}/src/*.cpp")

# Create the rplidar_sdk library
add_library(rplidar_sdk STATIC ${RPLIDAR_SDK_SRC})
target_include_directories(rplidar_sdk PUBLIC ${RPLIDAR_SDK_PATH}/include
                                              ${RPLIDAR_SDK_PATH}/src)
target_link_libraries(rplidar_sdk Threads::Threads)

# Create the rix_lib library
add_library(rix_lib rix/src/rix/core/node.cpp
                    rix/src/rix/core/publisher/publisher.cpp
                    rix/src/rix/core/publisher/pub_impl_base.cpp
                    rix/src/rix/core/publisher/pub_impl_tcp.cpp
                    rix/src/rix/core/subscriber/subscriber.cpp
                    rix/src/rix/core/subscriber/sub_impl_base.cpp
                    rix/src/rix/core/subscriber/sub_impl_tcp.cpp)
target_include_directories(rix_lib PRIVATE rix/include/ include/)
target_link_libraries(rix_lib PUBLIC Threads::Threads ${RIX_IPC_LIB} ${RIX_UTIL_LIB})

# Add the rixhub executable
add_executable(rixhub rixhub/src/mediator.cpp
                      rixhub/src/ledger.cpp
                      rixhub/src/rixhub.cpp)
target_include_directories(rixhub PRIVATE rix/include/ rixhub/include/ include/)
target_link_libraries(rixhub PRIVATE rix_lib)

# Add the lidar_driver
add_executable(lidar_driver lidar_driver/src/lidar.cpp
                            lidar_driver/src/lidar_driver.cpp)
target_link_libraries(lidar_driver PRIVATE rplidar_sdk rix_lib)
target_include_directories(lidar_driver PRIVATE rix/include lidar_driver/include include/)

# Add the mbot_driver
add_executable(mbot_driver mbot_driver/src/mbot.cpp
                           mbot_driver/src/mbot_driver.cpp)
target_link_libraries(mbot_driver PRIVATE rix_lib)
target_include_directories(mbot_driver PRIVATE rix/include mbot_driver/include include/)

# Add the teleop_keyboard executable
add_executable(teleop_keyboard src/teleop_keyboard.cpp)
target_link_libraries(teleop_keyboard PRIVATE rix_lib)
target_include_directories(teleop_keyboard PRIVATE rix/include/ include/)

# Add the slam_pose_subscriber executable
add_executable(slam_pose_listener src/slam_pose_listener.cpp)
target_link_libraries(slam_pose_listener PRIVATE rix_lib)
target_include_directories(slam_pose_listener PRIVATE rix/include/ include/)

# Add the timesync executable
add_executable(timesync src/timesync.cpp)
target_link_libraries(timesync PRIVATE rix_lib)
target_include_directories(timesync PRIVATE rix/include/ include/)

# Add the listener executable
add_executable(listener src/listener.cpp)
target_link_libraries(listener PRIVATE rix_lib)
target_include_directories(listener PRIVATE rix/include/ include/)

# Add the talker executable
add_executable(talker src/talker.cpp)
target_link_libraries(talker PRIVATE rix_lib)
target_include_directories(talker PRIVATE rix/include/ include/)

# Add the lidar_listener executable
add_executable(lidar_listener src/lidar_listener.cpp)
target_link_libraries(lidar_listener PRIVATE rix_lib)
target_include_directories(lidar_listener PRIVATE rix/include/ include/)

# Add the odometry_listener executable
add_executable(odometry_listener src/odometry_listener.cpp)
target_link_libraries(odometry_listener PRIVATE rix_lib)
target_include_directories(odometry_listener PRIVATE rix/include/ include/)
