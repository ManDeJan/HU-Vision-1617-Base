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
    double sigma  = 0.5;
    kernel_type sobel_x = {{-1,0,1},
                           {-2,0,2},
                           {-1,0,1}};

    kernel_type sobel_y = {{-1,-2,-1},
                           { 0, 0, 0},
                           { 1, 2, 1}};

    kernel_type gausKernel = produce2dGaussianKernel(radius, sigma);
    IntensityImage* blurImage = kernelApplyer(image, gausKernel);
    IntensityImage* gradiantX = kernelApplyer(*blurImage, sobel_x);
    IntensityImage* gradiantY = kernelApplyer(*blurImage, sobel_y);

    IntensityImage* combined  = combineImage(*gradiantX, *gradiantY);




    //saveIntensityImage()

    return combined;
}

IntensityImage *StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
    return nullptr;
}
