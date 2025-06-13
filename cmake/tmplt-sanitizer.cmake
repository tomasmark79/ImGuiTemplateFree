
# MIT License
# Copyright (c) 2024-2025 Tomáš Mark

function(apply_address_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    if (NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address -static-libasan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address)
    endif()
    set(ENV{ASAN_OPTIONS} "detect_leaks=1:strict_string_checks=1:verbosity=1:log_threads=1")
endfunction()

function(apply_undefined_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
    if (NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=undefined -static-libubsan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=undefined)
    endif()
endfunction()

function(apply_thread_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=thread)
    if (NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=thread -static-libtsan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=thread -static-libtsan)
    endif()
endfunction()

function(apply_memory_sanitizer TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=memory)
    if (NOT APPLE)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=memory -static-libmsan)
    else()
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=memory -static-libmsan)
    endif()
endfunction()

function(apply_sanitizers TARGET_NAME)
    message(STATUS "Sanitizer target=${TARGET_NAME}")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

        if(SANITIZE_ADDRESS)
            apply_address_sanitizer(${TARGET_NAME})
        endif()

        if(SANITIZE_UNDEFINED)
            apply_undefined_sanitizer(${TARGET_NAME})
        endif()

        if(SANITIZE_THREAD)
            if(SANITIZE_ADDRESS)
                message(FATAL_ERROR "Thread sanitizer is not compatible with Address sanitizer")
            endif()
            apply_thread_sanitizer(${TARGET_NAME})
        endif()

        if(SANITIZE_MEMORY)
            if(SANITIZE_ADDRESS OR SANITIZE_THREAD)
                message(FATAL_ERROR "Memory sanitizer is not compatible with Address or Thread sanitizer")
            endif()
            apply_memory_sanitizer(${TARGET_NAME})
        endif()

    else()
        message(WARNING "Sanitizers are only supported for GCC and Clang")
    endif()
endfunction()