#pragma once
#include "settings.h"

class ComplexNumber
{
private:
public:
    long double re;
    long double im;

    int xImage;
    int yImage;

    ComplexNumber() {}
    ComplexNumber(long double re, long double im) : re(re), im(im) {}
    ComplexNumber(int xImage, int yImage, long double re_min, long double re_max, long double im_min, long double im_max) : xImage(xImage), yImage(yImage)
    {
        re = re_min + xImage / (long double)settings::IMAGE_WIDTH * (re_max - re_min);
        im = im_min + yImage / (long double)settings::IMAGE_HEIGHT * (im_max - im_min);
    }
    int GetXImage(long double re_min, long double re_max)
    {
        return (int)((re - re_min) * settings::IMAGE_WIDTH / (re_max - re_min));
    }

    int GetYImage(long double im_min, long double im_max)
    {
        return (int)((im - im_min) * settings::IMAGE_HEIGHT / (im_max - im_min));
    }

    ComplexNumber Pow()
    {
        return {re * re - im * im, 2 * re * im};
    }

    ComplexNumber operator+(ComplexNumber c)
    {
        return {c.re + re, c.im + im};
    }
};