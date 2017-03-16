#pragma once
#include "StudentPreProcessing.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

double gaussian(double x, double mu, double sigma) {
    return std::exp(-(((x - mu) / (sigma)) * ((x - mu) / (sigma))) / 2.0);
}

typedef std::vector<double>     kernel_row;
typedef std::vector<kernel_row> kernel_type;

kernel_type produce2dGaussianKernel(int kernelRadius, double sigma = 1) {
    // double      sigma = kernelRadius / 2.;
    kernel_type kernel2d(2 * kernelRadius + 1, kernel_row(2 * kernelRadius + 1));
    double      sum = 0;

    // compute values
    for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++) {
            double x = gaussian(row, kernelRadius, sigma) * gaussian(col, kernelRadius, sigma);
            kernel2d[row][col] = x;
            sum += x;
        }
    }

    // normalize
    for (int row = 0; row < kernel2d.size(); row++) {
        for (int col = 0; col < kernel2d[row].size(); col++) {
            kernel2d[row][col] /= sum;
        }
    }
    return kernel2d;
}

void kernelPrinter(kernel_type kernel) {
    std::cout << std::setprecision(5) << std::fixed;
    for (int row = 0; row < kernel.size(); row++) {
        for (int col = 0; col < kernel[row].size(); col++) {
            std::cout << kernel[row][col] << ' ';
        }
        std::cout << '\n';
    }
}

IntensityImage* combineImage(const IntensityImage &image1, const IntensityImage &image2) {
    if(image1.getWidth() != image2.getWidth() || image1.getHeight() != image2.getHeight()); {
        // throw "ALLES GAAT FOUT ALLES BRAND WAAROM DOE JE DIT";
        std::cout << "Non matching imageheights" << '\n';
        std::cout << "x1, y1: " << image1.getWidth() << ", " << image1.getHeight() << '\n';
        std::cout << "x2, y2: " << image2.getWidth() << ", " << image2.getHeight() << '\n';
    }

    IntensityImage *returnImage = ImageFactory::newIntensityImage(image1.getWidth(), image1.getHeight());

    for (size_t i = 0; i < image1.getHeight(); i++) {
        for (size_t j = 0; j < image1.getWidth(); j++) {
            int value = sqrt(pow(image1.getPixel(j, i), 2) + pow(image2.getPixel(j, i), 2));
            if(value > 255) value = 255;
            
            returnImage->setPixel(j, i, value);
        }
    }
    return returnImage;
}

IntensityImage* kernelApplyer(const IntensityImage &image, kernel_type kernel) {
    // Print kernel
    uint kwidth = kernel.size();
    uint kheight = kwidth;
    kernelPrinter(kernel);

    // IntensityImagePrivate returnImage = IntensityImagePrivate(image.getWidth(), image.getHeight());
    IntensityImage *returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());

    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {
            int newPekel = 0;
            for (int i = 0; i < kheight; i++) {
                for (int j = 0; j < kwidth; j++) {
                    int realX = x + j - kwidth/2;
                    int realY = y + i - kheight/2;
                    if (realX < 0) realX = 0;
                    if (realX > image.getWidth()) realX = image.getWidth();
                    if (realY < 0) realY = 0;
                    if (realY > image.getHeight()) realY = image.getHeight();

                    newPekel += image.getPixel(realX, realY) * kernel[i][j];
                    // std::cout << realX << ' ' << realY << '\n';
                }
            }
            if(newPekel > 255) newPekel = 255;
            if(newPekel < 0) newPekel = 0;

            returnImage->setPixel(x, y, newPekel);
        }
    }


    return returnImage;
}
