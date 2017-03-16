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
    int radius = 5;
    double sigma  = 2;
    bool convolution = false;
    kernel_type sobel_xL = {{-1,0,1},
                            {-2,0,2},
                            {-1,0,1}};

    kernel_type sobel_xR = {{1,0,-1},
                            {2,0,-2},
                            {1,0,-1}};

    kernel_type sobel_yU = {{-1,-2,-1},
                            { 0, 0, 0},
                            { 1, 2, 1}};

    kernel_type sobel_yD = {{1,2,1},
                            { 0, 0, 0},
                            {-1,-2,-1}};

    kernel_type laplacian = {{0.5,1,0.5},
                             {1,-6,1},
                             {0.5,1,0.5}};


    edge_detec

    kernel_type gausKernel = produce2dGaussianKernel(radius, sigma);
    IntensityImage* blurImage = kernelApplyer(image, gausKernel);
//    IntensityImage* lapLacian = kernelApplyer(*blurImage, laplacian);
    IntensityImage* gradiantXR = kernelApplyer(*blurImage, sobel_xL, convolution);
    //IntensityImage* gradiantXL = kernelApplyer(*blurImage, sobel_xR);
    IntensityImage* gradiantYU = kernelApplyer(*blurImage, sobel_yU, convolution);
    //IntensityImage* gradiantYD = kernelApplyer(*blurImage, sobel_yD);

    //IntensityImage* combinedX  = combineImage(*gradiantXL, *gradiantXR);
    //IntensityImage* combinedY  = combineImage(*gradiantYU, *gradiantYD);
    IntensityImage* combined  = combineImage(*gradiantXR, *gradiantYU);


    //saveIntensityImage()

    return combined;
}

IntensityImage *StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
    return nullptr;
}
