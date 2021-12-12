project(CarpFishingShip)

set(Sources
    local_network_parse.c
    local_network_parse_tests.cpp
    main.cpp
)

add_executable(CarpFishingShip-TEST ${Sources})

add_test(
    NAME CarpFishingShip-TEST
    COMMAND CarpFishingShip-TEST
    )
#target_include_directories(TIC-TAC-TOE-TEST PUBLIC ../src) 
target_link_libraries(CarpFishingShip-TEST PUBLIC gtest_main)
