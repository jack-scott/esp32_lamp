import unittest
from rgb_hsv import ColorHSV, ColorRGB

class TestColorConversion(unittest.TestCase):
    def test_hsv_to_rgb_red(self):
        # Test case: Red color (h=0, s=255, v=255)
        hsv_color = ColorHSV(0, 255, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 255)
        self.assertEqual(rgb_color.g, 0)
        self.assertEqual(rgb_color.b, 0)

    def test_hsv_to_rgb_green(self):
        # Test case: Green color (h=120, s=255, v=255)
        hsv_color = ColorHSV(120, 255, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 0)
        self.assertEqual(rgb_color.g, 255)
        self.assertEqual(rgb_color.b, 0)

    def test_hsv_to_rgb_blue(self):
        # Test case: Blue color (h=240, s=255, v=255)
        hsv_color = ColorHSV(240, 255, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 0)
        self.assertEqual(rgb_color.g, 0)
        self.assertEqual(rgb_color.b, 255)

    def test_hsv_to_rgb_yellow(self):
        # Test case: Yellow color (h=60, s=255, v=255)
        hsv_color = ColorHSV(60, 255, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 255)
        self.assertEqual(rgb_color.g, 255)
        self.assertEqual(rgb_color.b, 0)

    def test_hsv_to_rgb_cyan(self):
        # Test case: Cyan color (h=180, s=255, v=255)
        hsv_color = ColorHSV(180, 255, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 0)
        self.assertEqual(rgb_color.g, 255)
        self.assertEqual(rgb_color.b, 255)

    def test_hsv_to_rgb_magenta(self):
        # Test case: Magenta color (h=300, s=255, v=255)
        hsv_color = ColorHSV(300, 255, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 255)
        self.assertEqual(rgb_color.g, 0)
        self.assertEqual(rgb_color.b, 255)

    def test_hsv_to_rgb_black(self):
        # Test case: Black color (h=0, s=0, v=0)
        hsv_color = ColorHSV(0, 0, 0)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 0)
        self.assertEqual(rgb_color.g, 0)
        self.assertEqual(rgb_color.b, 0)

    def test_hsv_to_rgb_white(self):
        # Test case: White color (h=0, s=0, v=255)
        hsv_color = ColorHSV(0, 0, 255)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 255)
        self.assertEqual(rgb_color.g, 255)
        self.assertEqual(rgb_color.b, 255)

    def test_hsv_to_rgb_gray(self):
        # Test case: Gray color (h=0, s=0, v=128)
        hsv_color = ColorHSV(0, 0, 128)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 128)
        self.assertEqual(rgb_color.g, 128)
        self.assertEqual(rgb_color.b, 128)

    def test_hsv_to_rgb_random(self):
        # Test case: Random color (h=180, s=128, v=64)
        hsv_color = ColorHSV(180, 128, 64)
        rgb_color = hsv_color.to_rgb()
        self.assertEqual(rgb_color.r, 31)
        self.assertEqual(rgb_color.g, 64)
        self.assertEqual(rgb_color.b, 64)



if __name__ == '__main__':
    unittest.main()