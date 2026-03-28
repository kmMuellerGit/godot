#[[#ifndef]]# HEADER_GUARD_${NAME.toUpperCase()}_H
#[[#define]]# HEADER_GUARD_${NAME.toUpperCase()}_H

#[[#include]]# "core/io/resource.h"
#[[#include]]# "game_common/gdscript_helpers.h"
#[[#include]]# "tests/test_macros.h"
#[[#include]]# "game_main/game_core_setup/GameMainTestHelpers.h"



/*
https://docs.godotengine.org/en/stable/contributing/development/core_and_modules/unit_testing.html

Logging
https://github.com/doctest/doctest/blob/master/doc/markdown/logging.md
    CAPTURE(some_variable)   ->    some_variable := 42
    INFO("lots of captures: ", some_var, " ", some_var, " ", some_var, ";");
    INFO("old way of capturing - using the streaming operator: " << some_var << " " << some_var);

    FAIL_CHECK(message) -> Fail test case, log message, but continue
    FAIL(message) -> Fail test case, stop

    // Subcases rerun entire TEST_CASE but only enter specific subcase portion.
*/
namespace TestingGame {

    TEST_CASE("[DungeonARPG][${TestNameSpace}][TEST CASE NAME 1]") {
        // Load in dummy scene
        Node3D *scene_root = GameMainTestHelpers::GameReady_LoadScene(GameMainTestHelpers::BLANK_100M_100M);
        GameMainNode* gmn = GameMainNode::get_singleton();
        // Do work
        gmn->get_raycast_collision_map()->DirtyWorldBounds(Vector3(-100,-100,-100), Vector3(100,100,100));
        // Pass frames
        for (int i = 0 ; i <50 ; ++i){
            GameMainTestHelpers::YieldFrame(0.1);
        }
        // Cleanup
        scene_root->queue_free();
    }
    
    TEST_CASE("[DungeonARPG][${TestNameSpace}][TEST CASE NAME 1]") {
        Node* node = memnew(Node);
        node->set_name("some name");

        SUBCASE("RUNS_TEST_PER_SUBCASE"){
            CHECK_MESSAGE(node->get_name() == "some name", "Wrong name");
        }
        memdelete(node);
    }
    
    
    TEST_CASE("[DungeonARPG][${TestNameSpace}][EXAMPLE LINES]") {
        SUBCASE("Regen to max amount2"){
            float myFloat = 1.00001f;
            CHECK_MESSAGE(
                myFloat == doctest::Approx(1.0f),
                "The returned HSV saturation should match the expected value.");
            CHECK_EQ(2, 2);
            
            ERR_PRINT_OFF;
            // Do illegal thing here
            ERR_PRINT_ON;
        }
        
        SUBCASE("Regen to max amount2"){
            // Access node, process time
            Timer *test_timer = memnew(Timer);
            SceneTree::get_singleton()->get_root()->add_child(test_timer);

            test_timer->set_process(true);
            test_timer->set_physics_process(true);
            SceneTree::get_singleton()->process(0.2);
            // Do work
            memdelete(test_timer); // Cleanup nodes
        }
        
        SUBCASE("Resource Instantiate - no need to clean up"){
            Ref<Sky> test_sky;
            test_sky.instantiate();
        }
    }
}

#endif // HEADER_GUARD_${NAME.toUpperCase()}_H