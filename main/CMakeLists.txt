idf_component_register(SRCS "main.cpp" "SR04.cpp"
                    PRIV_REQUIRES spi_flash
                    REQUIRES driver esp_timer
                    INCLUDE_DIRS "")
