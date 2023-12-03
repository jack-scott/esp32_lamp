#include <hsv_to_rgb.h>
#include <gtest/gtest.h>
#include <iostream>
#include <cmath>

// Test the toRGB method of the ColorHSV class
TEST(ColorHSVTest, initTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{120.0, 0.5, 0.8});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 120.0);
    EXPECT_FLOAT_EQ(color.get_s(), 0.5);
    EXPECT_FLOAT_EQ(color.get_v(), 0.8);
}

// Test all valid h input range in loop
TEST(ColorHSVTest, hRangeTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{0, 0.5, 0.8});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 0);
    EXPECT_FLOAT_EQ(color.get_s(), 0.5);
    EXPECT_FLOAT_EQ(color.get_v(), 0.8);

    // Loop through all valid h values
    for (int i = 0; i < 360; i++) {
        // Set the HSV values
        color.set_h(i);
        // Check the expected HSV values
        EXPECT_FLOAT_EQ(color.get_h(), i);
        EXPECT_FLOAT_EQ(color.get_s(), 0.5);
        EXPECT_FLOAT_EQ(color.get_v(), 0.8);
    }
}

// Test all valid s input range in loop
TEST(ColorHSVTest, sRangeTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{120, 0, 0.8});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 120);
    EXPECT_FLOAT_EQ(color.get_s(), 0);
    EXPECT_FLOAT_EQ(color.get_v(), 0.8);

    // Loop through all valid s values
    for (int i = 0; i < 256; i++) {
        // Set the HSV values
        color.set_s(i);
        // Check the expected HSV values
        EXPECT_FLOAT_EQ(color.get_h(), 120);
        EXPECT_FLOAT_EQ(color.get_s(), i);
        EXPECT_FLOAT_EQ(color.get_v(), 0.8);
    }
}

// Test all valid v input range in loop
TEST(ColorHSVTest, vRangeTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{120, 0.5, 0});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 120);
    EXPECT_FLOAT_EQ(color.get_s(), 0.5);
    EXPECT_FLOAT_EQ(color.get_v(), 0);

    // Loop through all valid v values
    for (int i = 0; i < 256; i++) {
        // Set the HSV values
        color.set_v(i);
        // Check the expected HSV values
        EXPECT_FLOAT_EQ(color.get_h(), 120);
        EXPECT_FLOAT_EQ(color.get_s(), 0.5);
        EXPECT_FLOAT_EQ(color.get_v(), i);
    }
}

// Test all valid h input range with bounds applied in loop
TEST(ColorHSVTest, hRangeBoundsTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{0, 0.5, 0.8});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 0);
    EXPECT_FLOAT_EQ(color.get_s(), 0.5);
    EXPECT_FLOAT_EQ(color.get_v(), 0.8);

    // Loop through all valid h values
    for (int i = 0; i < 360; i++) {

        // Apply bounds
        HSV_float new_colour = color.applyBounds(HSV_float{i, 0.5, 0.8});
        // Set the HSV values
        color.set_h(new_colour.h);
        
        // Check the expected HSV values
        EXPECT_FLOAT_EQ(color.get_h(), i);
        EXPECT_FLOAT_EQ(color.get_s(), 0.5);
        EXPECT_FLOAT_EQ(color.get_v(), 0.8);
    }
}

// test functionality of the std::remainderf function
TEST(ColorHSVTest, remainderTest) {
    //just use floats and the std::remainderf function
    float h = 0.0f;
    float h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 0.0f);

    h = 360.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 0.0f);

    h = 720.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 0.0f);

    h = 10.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 10.0f);

    h = 370.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 10.0f);

    h = 100.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 100.0f);

    h = 150.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 150.0f);

    h = 180.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 180.0f);

    h = 190.0f;
    h_remainder = fmodf(h, 360.0f);
    EXPECT_FLOAT_EQ(h_remainder, 190.0f); 
}


// Test setting and getting components
TEST(ColorHSVTest, setGetTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{120, 0.5, 0.8});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 120);
    EXPECT_FLOAT_EQ(color.get_s(), 0.5);
    EXPECT_FLOAT_EQ(color.get_v(), 0.8);
    // Set the HSV values
    color.set_h(240);
    color.set_s(0.2);
    color.set_v(0.4);
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 240);
    EXPECT_FLOAT_EQ(color.get_s(), 0.2);
    EXPECT_FLOAT_EQ(color.get_v(), 0.4);
}


//Test bounds functions
TEST(ColorHSVTest, boundsTest) {
    // Create a ColorHSV object with HSV values
    ColorHSV color(HSV_float{380, -0.5, 260});
    // Check the expected HSV values
    EXPECT_FLOAT_EQ(color.get_h(), 20);
    EXPECT_FLOAT_EQ(color.get_s(), 0);
    EXPECT_FLOAT_EQ(color.get_v(), 255);

    // Set the HSV values
    color.set_h(-20);
    color.set_s(1.5);
    color.set_v(-10);

    // Check that no bounds have been applied
    EXPECT_FLOAT_EQ(color.get_h(), -20);
    EXPECT_FLOAT_EQ(color.get_s(), 1.5);
    EXPECT_FLOAT_EQ(color.get_v(), -10);

    // Apply bounds
    HSV_float new_colour = color.applyBounds(HSV_float{-20, 1.5, -10});

    // Check the expected HSV values
    EXPECT_FLOAT_EQ(new_colour.h, 0);
    EXPECT_FLOAT_EQ(new_colour.s, 1.5);
    EXPECT_FLOAT_EQ(new_colour.v, 0);
}

// Test the toRGB method of the ColorHSV class
TEST(ColorHSVTest, test_hsv_to_rgb_red) {
    // Test case: Red color (h=0, s=255, v=255)
    ColorHSV hsv_colour(HSV_float{0, 255, 255});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 255);
    EXPECT_EQ(rgb_colour.g, 0);
    EXPECT_EQ(rgb_colour.b, 0);
}

TEST(ColorHSVTest, test_hsv_to_rgb_green) {
    // Test case: Green color (h=120, s=255, v=255)
    ColorHSV hsv_colour(HSV_float{120, 255, 255});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 0);
    EXPECT_EQ(rgb_colour.g, 255);
    EXPECT_EQ(rgb_colour.b, 0);
}

TEST(ColorHSVTest, test_hsv_to_rgb_blue) {
    // Test case: Blue colour (h=240, s=255, v=255)
    ColorHSV hsv_colour(HSV_float{240.0, 255.0, 255.0});

    // Test hsv 
    EXPECT_FLOAT_EQ(hsv_colour.get_h(), 240.0);
    EXPECT_FLOAT_EQ(hsv_colour.get_s(), 255.0);
    EXPECT_FLOAT_EQ(hsv_colour.get_v(), 255.0);


    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 0);
    EXPECT_EQ(rgb_colour.g, 0);
    EXPECT_EQ(rgb_colour.b, 255);
}

TEST(ColorHSVTest, test_hsv_to_rgb_yellow) {
    // Test case: Yellow colour (h=60, s=255, v=255)
    ColorHSV hsv_colour(HSV_float{60, 255, 255});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 255);
    EXPECT_EQ(rgb_colour.g, 255);
    EXPECT_EQ(rgb_colour.b, 0);
}

TEST(ColorHSVTest, test_hsv_to_rgb_cyan) {
    // Test case: Cyan colour (h=180, s=255, v=255)
    ColorHSV hsv_colour(HSV_float{180, 255, 255});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 0);
    EXPECT_EQ(rgb_colour.g, 255);
    EXPECT_EQ(rgb_colour.b, 255);
}

TEST(ColorHSVTest, test_hsv_to_rgb_magenta) {
    // Test case: Magenta colour (h=300, s=255, v=255)
    ColorHSV hsv_colour(HSV_float{300, 255, 255});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 255);
    EXPECT_EQ(rgb_colour.g, 0);
    EXPECT_EQ(rgb_colour.b, 255);
}

TEST(ColorHSVTest, test_hsv_to_rgb_black) {
    // Test case: Black colour (h=0, s=0, v=0)
    ColorHSV hsv_colour(HSV_float{0, 0, 0});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 0);
    EXPECT_EQ(rgb_colour.g, 0);
    EXPECT_EQ(rgb_colour.b, 0);
}

TEST(ColorHSVTest, test_hsv_to_rgb_white) {
    // Test case: White colour (h=0, s=0, v=255)
    ColorHSV hsv_colour(HSV_float{0, 0, 255});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 255);
    EXPECT_EQ(rgb_colour.g, 255);
    EXPECT_EQ(rgb_colour.b, 255);
}

TEST(ColorHSVTest, test_hsv_to_rgb_gray) {
    // Test case: Gray colour (h=0, s=0, v=128)
    ColorHSV hsv_colour(HSV_float{0, 0, 128});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 128);
    EXPECT_EQ(rgb_colour.g, 128);
    EXPECT_EQ(rgb_colour.b, 128);
}

TEST(ColorHSVTest, test_hsv_to_rgb_random) {
    // Test case: Random colour (h=180, s=128, v=64)
    ColorHSV hsv_colour(HSV_float{180, 128, 64});
    RGB_colour rgb_colour = hsv_colour.toRGB();
    EXPECT_EQ(rgb_colour.r, 31);
    EXPECT_EQ(rgb_colour.g, 64);
    EXPECT_EQ(rgb_colour.b, 64);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}