function i = calcInertia(d, h, t)
    i = 9.807 * 1.548 * (.0254 * d)^2 * t^2 / (16*pi^2*.0254*h);
end