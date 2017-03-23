#pragma once
#include "ImageIO.h"
#include "StudentPreProcessing.h"
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
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

// This function is no longer needed
IntensityImage *combineImage(const IntensityImage &image1, const IntensityImage &image2) {
    if (image1.getWidth() != image2.getWidth() || image1.getHeight() != image2.getHeight()) {
        // throw "ALLES GAAT FOUT ALLES BRAND WAAROM DOE JE DIT";
        std::cout << "Non matching imageheights" << '\n';
        std::cout << "x1, y1: " << image1.getWidth() << ", " << image1.getHeight() << '\n';
        std::cout << "x2, y2: " << image2.getWidth() << ", " << image2.getHeight() << '\n';
    }

    IntensityImage *returnImage = ImageFactory::newIntensityImage(image1.getWidth(), image1.getHeight());

    for (size_t i = 0; i < image1.getHeight(); i++) {
        for (size_t j = 0; j < image1.getWidth(); j++) {
            int value = sqrt(pow(image1.getPixel(j, i), 2) + pow(image2.getPixel(j, i), 2));
            if (value >= 255) value = 255;
            if (value <= 0) value = 0;
            returnImage->setPixel(j, i, value);
        }
    }
    return returnImage;
}

using gradientImage = std::vector<std::vector<double>>;
using refinedGradientImage = std::vector<std::vector<uint8_t>>;

IntensityImage *kernelApplyer(const IntensityImage &image, std::vector<kernel_type> kernels,
                              std::function<int(std::vector<int>)> merge =
                              [](std::vector<int> a) {
                                  int total = 0;
                                  for (auto x : a) {
                                      total += x;
                                  }
                                  return total;
                              },
                              gradientImage *gradientDirection = nullptr) {
    // Print kernel

    uint kwidth = kernels[0].size();
    uint kheight = kwidth;

    IntensityImage *returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
    if (gradientDirection) {
        gradientDirection->resize(image.getWidth());
        for (size_t i = 0; i < image.getWidth(); i++) {
            (*gradientDirection)[i].resize(image.getHeight());
        }
    }

    int ducks = 0;
    int kernelAmount = kernels.size();

    for (int y = 0; y < image.getHeight(); y++) {
        for (int x = 0; x < image.getWidth(); x++) {

            std::vector<int> newPekels = { 0 };
            int              newPekel = 0;
            newPekels.resize(kernelAmount);
            for (int i = 0; i < kheight; i++) {
                for (int j = 0; j < kwidth; j++) {

                    // bounds checking
                    int realX = x + j - kwidth / 2;
                    int realY = y + i - kheight / 2;
                    if (realX < 0) realX = 0;
                    if (realX > image.getWidth()) realX = image.getWidth();
                    if (realY < 0) realY = 0;
                    if (realY > image.getHeight()) realY = image.getHeight();

                    for (size_t k = 0; k < kernelAmount; k++) {
                        newPekels[k] += image.getPixel(realX, realY) * kernels[k][i][j];
                    }
                }
            }
            if (gradientDirection) {
                double direction = (atan2(newPekels[0], newPekels[1]) * 180 / M_PI);
                if (direction < 0) direction += 180;
                (*gradientDirection)[x][y] = direction;
            }
            newPekel = merge(newPekels);

            if (newPekel > 255) newPekel = 255;
            if (newPekel < 0) newPekel = 0;

            returnImage->setPixel(x, y, (uint8_t)newPekel);
        }
    }

    return returnImage;
}

refinedGradientImage gradientRefiner(gradientImage &x) {
    refinedGradientImage image;

    image.resize(x.size());
    for (size_t i = 0; i < x.size(); i++) {
        image[i].resize(x[0].size());
    }

    for (size_t i = 0; i < x.size(); i++) {
        for (size_t j = 0; j < x[i].size(); j++) {
            double  direction = x[i][j];
            uint8_t refined = 0;
            if (direction > 22.5 && direction < 67.5) {
                refined = 45;
            } else if (direction > 67.5 && direction < 112.5) {
                refined = 90;
            } else if (direction > 112.5 && direction < 157.5) {
                refined = 135;
            }
            image[i][j] = refined;
        }
    }

    return image;
}

IntensityImage *imageSurpressor(const IntensityImage &image, const refinedGradientImage &hoekies) {
    IntensityImage *returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
    for (size_t i = 0; i < image.getHeight() * image.getWidth(); i++) {
        returnImage->setPixel(i, image.getPixel(i));
    }

    std::cout << "sizeof image en hoekies ding: " << hoekies.size() << ' ' << image.getWidth() << '\n';
    std::cout << "sizeof image en hoekies ding: " << hoekies[0].size() << ' ' << image.getHeight() << '\n';

    for (uint i = 0; i < image.getHeight(); i++)
        for (uint j = 0; j < image.getWidth(); j++) {
            int ax, ay, bx, by;
            int hoek = hoekies[j][i];
            if (hoek == 0) {
                ax = j + 1;
                ay = i;
                bx = j - 1;
                by = i;
            } else if (hoek == 45) {
                ax = j + 1;
                ay = i + 1;
                bx = j - 1;
                by = i - 1;
            } else if (hoek == 90) {
                ax = j;
                ay = i + 1;
                bx = j;
                by = i - 1;
            } else /* if (hoek == 135) */ {
                ax = j - 1;
                ay = i + 1;
                bx = j + 1;
                by = i - 1;
            }

            if (ax < 0 || ax >= image.getWidth() || ay < 0 || ay >= image.getHeight())
                continue;
            else if (image.getPixel(ax, ay) > image.getPixel(j, i)) {
                returnImage->setPixel(j, i, 0);
                continue;
            }

            if (bx < 0 || bx >= image.getWidth() || by < 0 || by >= image.getHeight())
                continue;
            else if (image.getPixel(bx, by) > image.getPixel(j, i)) {
                returnImage->setPixel(j, i, 0);
                continue;
            }
        }
    return returnImage;
}

IntensityImage *hysterisch(const IntensityImage &image, int thresholdLow, int thresholdHigh) {
    using ints = std::pair<int, int>;
    IntensityImage * returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
    std::queue<ints> nodes;

    for (size_t i = 0; i < image.getHeight(); i++) {
        for (size_t j = 0; j < image.getWidth(); j++) {
            if ((image.getPixel(j, i) >= thresholdHigh) && (returnImage->getPixel(j, i) != 255)) {
                nodes.push(ints{ j, i });
                while (!nodes.empty()) {
                    ints node = nodes.front();
                    nodes.pop();
                    int x = node.first, y = node.second;
                    if (x < 0 || x >= image.getWidth() || y < 0 || y >= image.getHeight()) continue;
                    if (image.getPixel(x, y) < thresholdLow) continue;
                    if (returnImage->getPixel(x, y) != 255) {
                        returnImage->setPixel(x, y, 255);
                        nodes.push(ints{x+1,y-1});
                        nodes.push(ints{x+1,y  });
                        nodes.push(ints{x+1,y+1});
                        nodes.push(ints{x  ,y+1});
                        nodes.push(ints{x  ,y-1});
                        nodes.push(ints{x-1,y-1});
                        nodes.push(ints{x-1,y  });
                        nodes.push(ints{x-1,y+1});
                    }
                }
            }
        }
    }


    return returnImage;
}
