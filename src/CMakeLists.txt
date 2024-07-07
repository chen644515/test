aux_source_directory (. SRC_LIST)

find_package(
    OpenCV REQUIRED
)

include_directories (
    ../include
    /usr/local/include
    /usr/include/opencv4
    /usr/include/gstreamer-1.0
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
)

# add_executable (image_send image_send.cpp)
# target_link_libraries(image_send
#   ${OpenCV_LIBS}
#   -lglib-2.0 -lgobject-2.0 -lgmodule-2.0
#   -lgstreamer-1.0
#   -lgstapp-1.0 -lgstbase-1.0
# )

# add_executable (image_receive image_receive)
# target_link_libraries(image_receive
#   ${OpenCV_LIBS}
#   -lglib-2.0 -lgobject-2.0 -lgmodule-2.0
#   -lgstreamer-1.0
#   -lgstapp-1.0 -lgstbase-1.0
# )

add_executable (image_rev_tcp image_rev_tcp)
target_link_libraries(image_rev_tcp
  ${OpenCV_LIBS}
)

add_executable (image_send_tcp image_send_tcp)
target_link_libraries(image_send_tcp
  ${OpenCV_LIBS}
)

# add_executable (image_send_udp image_send_udp)
# target_link_libraries(image_send_udp
#   ${OpenCV_LIBS}
# )

# add_executable (image_rev_udp image_rev_udp)
# target_link_libraries(image_rev_udp
#   ${OpenCV_LIBS}
# )

# add_executable (image_send_udp_test image_send_udp_test)
# target_link_libraries(image_send_udp_test
#   ${OpenCV_LIBS}
# )

# add_executable (vdma1-test vdma1-test)
# target_link_libraries(vdma1-test
#   ${OpenCV_LIBS}
# )

add_executable (image_rev_tcp_mask image_rev_tcp_mask)
target_link_libraries(image_rev_tcp_mask
  ${OpenCV_LIBS}
)

add_executable (image_send_tcp_mask image_send_tcp_mask)
target_link_libraries(image_send_tcp_mask
  ${OpenCV_LIBS}
)

add_executable (image_rev_tcp_ask image_rev_tcp_ask)
target_link_libraries(image_rev_tcp_ask
  ${OpenCV_LIBS}
)

add_executable (image_send_tcp_ask image_send_tcp_ask)
target_link_libraries(image_send_tcp_ask
  ${OpenCV_LIBS}
)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
