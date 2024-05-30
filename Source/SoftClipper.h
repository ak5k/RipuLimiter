#pragma once

template <typename T>
T SoftClipper(T x, T drive)
{
    x *= drive;
    if (x < -1)
        return -1 / drive;

    if (x > 1)
        return 1 / drive;

    (x - x * x * x / 3) * 3 / 2 / drive;
}