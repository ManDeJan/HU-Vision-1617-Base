#include "StudentPreProcessing.h"
#include "ImageFactory.h"
#include "IntensityImagePrivate.h"
#include "JanEnDavidFunctionHouse.h"


IntensityImage *StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
    return nullptr;
}

IntensityImage *StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
    return nullptr;
}

// Here we do edgy detection.
IntensityImage *StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
    // First we blur.
    int radius = 2;
    double sigma  = 1;

    kernel_type sobel_X = {{-1,0,1},
                           {-2,0,2},
                           {-1,0,1}};

    kernel_type sobel_Y = {{-1,-2,-1},
                           { 0, 0, 0},
                           { 1, 2, 1}};

    kernel_type laplacian = {{0.5 ,1, 0.5},
                             {1,  -6,   1},
                             {0.5, 1, 0.5}};

    kernel_type gausKernel = produce2dGaussianKernel(radius, sigma);

    IntensityImage* blurImage = kernelApplyer(image, {gausKernel});

    auto sobel_merge = [](std::vector<int> pekkels) {
        return sqrt(pow(pekkels[0], 2) + pow(pekkels[1], 2));
    };

    IntensityImage* sobelImage = kernelApplyer(*blurImage, {sobel_X, sobel_Y}, sobel_merge);

    // IntensityImage* lapLacian = kernelApplyer(*blurImage, laplacian);

    return sobelImage;
}

IntensityImage *StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
    return nullptr;
}
