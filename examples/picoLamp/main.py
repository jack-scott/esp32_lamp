# Imports
import time
from machine import Pin
from neopixel import NeoPixel
import random


def sweep(input_val, min, max, increment):
    if increment <= 0:
        print("Increment must be greater than 0")
        return input_val
    if input_val < min:
        input_val = min
    elif input_val < max:
        input_val += increment
    else:
        input_val = min
    return input_val

def bounce(input_val, last_direction, min, max, increment):
    if input_val < max and last_direction == 1:
        input_val += increment
    elif input_val >= max and last_direction == 1:
        input_val -= increment
        last_direction = -1
    elif input_val > min and last_direction == -1:
        input_val -= increment
    elif input_val <= min and last_direction == -1:
        input_val += increment
        last_direction = 1
    return input_val, last_direction


class ColorHSV:
    # Clip H to the range 0-360
    # Clip S to the range 0-255
    # Clip V to the range 0-255
    def __init__(self, h, s, v):
        self.h = h
        _, self.s, self.v = self.apply_bounds(h, s, v)

    def apply_bounds(self, h, s, v):
        h = h % 360
        s = min(255, max(0, s))
        v = min(255, max(0, v))
        return h, s, v
    
    def normalize(self, h, s, v):
        h = h
        s = s / 255
        v = v / 255
        return h, s, v

    def _hsv_to_component(self, h, s, v, k):
        return (v - v * s * max(min(k, 4 - k, 1), 0))

    def to_rgb(self):
        h, s, v = self.apply_bounds(self.h, self.s, self.v)
        h, s, v = self.normalize(h, s, v)

        r_k = (5 + h / 60) % 6
        r = self._hsv_to_component(h, s, v, r_k)

        g_k = (3 + h / 60) % 6
        g = self._hsv_to_component(h, s, v, g_k)

        b_k = (1 + h / 60) % 6
        b = self._hsv_to_component(h, s, v, b_k)

        return ColorRGB(int(r * 255), int(g * 255), int(b * 255))


class ColorRGB:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b


class DeliciousFrostyFruit:
    def __init__(self, strip, num_leds):
        self.strip = strip
        self.num_leds = num_leds

        self.h_min = 8
        self.h_max = 12
        self.s_min = 238
        self.s_max = 245
        self.v = 50

        h_start = random.randrange(self.h_min, self.h_max)
        s_start = random.randrange(self.s_min, self.s_max)

        self.cmd_colour = ColorHSV(h_start, s_start, self.v)
        self.update_speed = 0.08
        self.h_increment = 0.1
        self.s_increment = 0.05
        
    def run(self):
        h_last_direction = 1
        s_last_direction = 1
        led_last_direction = 1
        curr_led = 0

        while True: # Run forever
            curr_led, led_last_direction = bounce(curr_led, led_last_direction, 0, self.num_leds-1, 1)
            self.cmd_colour.h, h_last_direction = bounce(self.cmd_colour.h, h_last_direction, self.h_min, self.h_max, self.h_increment)
            self.cmd_colour.s, s_last_direction = bounce(self.cmd_colour.s, s_last_direction, self.s_min, self.s_max, self.s_increment)

            rgb_out = self.cmd_colour.to_rgb()
            
            self.strip[curr_led] = (rgb_out.r, rgb_out.g, rgb_out.b)
            print("LED: {}, H: {}, S: {}".format(curr_led, self.cmd_colour.h, self.cmd_colour.s))
            print("RGB: {}, {}, {}".format(rgb_out.r, rgb_out.g, rgb_out.b))
            print("")
            time.sleep(self.update_speed)
            self.strip.write()


# Define the strip pin number (28) and number of LEDs (15)
num_leds = 22
strip = NeoPixel(Pin(28), num_leds)



animation = DeliciousFrostyFruit(strip, num_leds)
animation.run()