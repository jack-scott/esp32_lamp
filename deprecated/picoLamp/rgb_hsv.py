
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
