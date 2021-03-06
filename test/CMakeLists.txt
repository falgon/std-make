#=============================================================================
# Setup unit tests
#=============================================================================
add_custom_target(tests COMMENT "Build all the unit tests.")


function(jasel_add_module module)
    add_custom_target(${module}
        COMMENT "Build unit tests for the ${module} module.")
    add_dependencies(tests ${module})
endfunction()

function(jasel_add_standalone_test test source)
    # We compile each unit test with the possible combinations of options
    # found in boost/jasel/detail/config.hpp.
    #
    # For lack of a better solution, we currently create different targets
    # for each configuration.
    # xxx  - JASEL_XXX
    add_custom_target(${test})

    macro(helper suffix)
        add_executable(${test}-${suffix} ${source})
        add_test(test.${test}-${suffix} ${test}-${suffix})
        add_dependencies(${test} ${test}-${suffix})


        foreach(def IN ITEMS ${ARGN})
            set_property(TARGET ${test}-${suffix}
                         APPEND PROPERTY COMPILE_DEFINITIONS ${def})
        endforeach()
    endmacro()
    helper(normal)
    #helper(xxx-yyyy JASEL_XXX                             JASEL_YYY)
endfunction()

function(jasel_add_tests module dir)
    if(NOT TARGET ${module})
        jasel_add_module(${module})
    endif()

    foreach(test IN ITEMS ${ARGN})
        jasel_add_standalone_test(${module}.${test} ${dir}/${test}.cpp)
        add_dependencies(${module} ${module}.${test})
    endforeach()
endfunction()


function(jasel_add_standalone_example example source)
    # We compile each unit test with the possible combinations of options
    # found in boost/jasel/detail/config.hpp.
    #
    # For lack of a better solution, we currently create different targets
    # for each configuration.
    # xxx  - JASEL_XXX
    add_custom_target(${example})

    macro(helper suffix)
        add_executable(${example}-${suffix} ${source})
        add_test(example.${example}-${suffix} ${example}-${suffix})
        add_dependencies(${example} ${example}-${suffix})


        foreach(def IN ITEMS ${ARGN})
            set_property(TARGET ${example}-${suffix}
                         APPEND PROPERTY COMPILE_DEFINITIONS ${def})
        endforeach()
    endmacro()
    helper(normal)
    #helper(xxx-yyyy JASEL_XXX                             JASEL_YYY)
endfunction()

function(jasel_add_examples module dir)
    if(NOT TARGET ${module})
        jasel_add_module(${module})
    endif()

    foreach(example IN ITEMS ${ARGN})
        jasel_add_standalone_example(${module}.${example} ../example/${dir}/${example}.cpp)
        add_dependencies(${module} ${module}.${example})
    endforeach()
endfunction()

#=============================================================================
# Test suites
#=============================================================================

jasel_add_tests(
    applicative applicative
    ######
    #applicative_pass
    applicative_std_array_pass
    applicative_std_expected_pass
    #applicative_std_optional_pass
    #applicative_std_pair_pass
    #applicative_std_shared_ptr_pass
    applicative_std_tuple_pass
    #applicative_std_unique_ptr_pass
)

jasel_add_tests(
    bits bits
    ######
    bit_pass
    bit_mask_pass
)

jasel_add_examples(
    bits bits
    ######
    bit
)

jasel_add_tests(
    callable_wrapper callable_wrapper
    ######
    #ex_pass # optional
)

jasel_add_tests(
    expected expected
    ######
    #expected_no_move_constructible_fail
    expected_pass
)

jasel_add_tests(
    factory make
    ######
    emplace_ready_future_pass
    #make_boost_expected_pass
    #make_boost_optional_pass
    make_pass
    make_ready_future_pass
    make_std_array_pass
    make_std_expected_pass
    make_std_future_pass
    #make_std_optional_pass
    make_std_pair_pass
    make_std_shared_future_pass
    make_std_shared_ptr_pass
    make_std_tuple_pass
    make_std_unique_ptr_pass
)

jasel_add_tests(
    functional functional
    ######
    overload_pass
)

jasel_add_tests(
    functor functor
    ######
    functor_pass
    functor_std_array_pass
    functor_std_expected_pass
    #functor_std_optional_pass
    functor_std_pair_pass
    functor_std_shared_ptr_pass
    functor_std_tuple_pass
    functor_std_unique_ptr_pass
)

jasel_add_tests(
    monad monad
    ######
    monad_std_expected_pass
    #monad_std_optinal_pass
)

jasel_add_tests(
    monad_error monad_error
    ######
    monad_error_std_expected_pass
    #monad_error_std_optinal_pass
    monad_error_std_unique_ptr_pass
)

jasel_add_tests(
    n_applicative n_applicative
    ######
    #n_applicative_pass
    #n_applicative_std_array_pass
    #n_applicative_std_pair_pass
    n_applicative_std_tuple_pass
)

jasel_add_tests(
    n_functor n_functor
    ######
    n_functor_pass
    #n_functor_std_array_pass
    n_functor_std_pair_pass
    n_functor_std_tuple_pass
)

jasel_add_tests(
    nullable nullable
    ######
    nullable_pass
    #nullable_std_optional_pass
    nullable_std_shared_ptr_pass
    nullable_std_unique_ptr_pass
)

jasel_add_tests(
    ordinal ordinal
    ######
    ordinal_pass
)

jasel_add_tests(
    ordinal.array ordinal/container/ordinal_array
    ######
    ordinal_array_pass
)

jasel_add_tests(
    ordinal.range ordinal/container/ordinal_range
    ######
    ordinal_range_pass
)

jasel_add_tests(
    ordinal.set ordinal/container/ordinal_set
    ######
    ordinal_set_pass
)

jasel_add_tests(
    optionals optionals
    ######
    #optionals_pass # optional
)

jasel_add_tests(
    p_applicative p_applicative
    ######
    #p_applicative_pass
    #p_applicative_std_array_pass
    #p_applicative_std_pair_pass
    p_applicative_std_tuple_pass
)

jasel_add_tests(
    p_functor p_functor
    ######
    p_functor_pass
    #p_functor_std_array_pass
    p_functor_std_pair_pass
    p_functor_std_tuple_pass
)
jasel_add_tests(
    pod.optional pod/optional
    ######
    optional_pass
)

jasel_add_tests(
    pod.optionals pod/optionals
    ######
    #pod_optionals_pass # optional
)

jasel_add_tests(
    product_type product_type
    ######
    c_array_pass
    std_array_pass
    std_pair_pass
    std_tuple_pass
    struct_pass
)

jasel_add_tests(
    range range
    ######
    range_c_array_pass
    range_std_array_pass
)

jasel_add_tests(
    scoped scoped
    ######
    scope_pass
)

jasel_add_tests(
    strong strong
    ######
    byte_pass
    strict_bool_pass
    strong_bool_pass
    strong_bounded_int_pass
    #strong_counter_pass #fixme #369
    #strong_enums_pass #fixme
    strong_id_pass
    strong_ints_pass
)

jasel_add_examples(
    strong strong/
    ######
    address_offset_pass
    duration_pass
    money
    money3
    strong_money
)

jasel_add_examples(
    chrono.frames chrono/
    ######
    frame_types_pass
    day_types_pass
)

jasel_add_tests(
    sum_type sum_type
    ######
    sum_type_pass
    sum_type_std_expected_pass
    #sum_type_std_optional_pass
)


jasel_add_tests(
    swappable swappable
    ######
    swap_c_array_pass
)


jasel_add_tests(
    utility utility
    ######
    lvalue_ref_pass
    out_param_pass
    only_when_pass
    not_null_pass
)

