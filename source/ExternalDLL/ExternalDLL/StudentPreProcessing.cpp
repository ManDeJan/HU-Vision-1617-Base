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
    int radius = 1;
    double sigma  = 0.5;

    kernel_type sobel_X = {{-1,0,1},
                           {-2,0,2},
                           {-1,0,1}};

    kernel_type sobel_Y = {{-1,-2,-1},
                           { 0, 0, 0},
                           { 1, 2, 1}};

    kernel_type fopKernel = {{ 0, 0, 0},
                             { 0, 1, 0},
                             { 0, 0, 0}};


    kernel_type laplacian = {{0.5 ,1, 0.5},
                             {1,  -6,   1},
                             {0.5, 1, 0.5}};

    kernel_type gausKernel = produce2dGaussianKernel(radius, sigma);

    IntensityImage* blurImage = kernelApplyer(image, {gausKernel});

    auto sobel_merge = [](std::vector<int> pekkels) {
        return sqrt(pow(pekkels[0], 2) + pow(pekkels[1], 2));
    };

    auto fop_merge = [](std::vector<int> pekkels) {
        return pekkels[1];
    };

    gradientImage gradientDirection;
    IntensityImage* sobelImage = kernelApplyer(*blurImage, {sobel_X, sobel_Y}, sobel_merge, &gradientDirection);

    // kernelPrinter(gradientDirection);

    // for (size_t i = 0; i < sobelImage->getHeight(); i++) {
    //     for (size_t j = 0; j < sobelImage->getWidth(); j++) {
    //         if (gradientDirection[j][i] > 0) {
    //             sobelImage->setPixel(j, i, 255);
    //         } else {
    //             sobelImage->setPixel(j, i, 0);
    //         }
    //
    //     }
    // }

    // IntensityImage* lapLacian = kernelApplyer(*blurImage, laplacian);

    refinedGradientImage refinedGradientDirection = gradientRefiner(gradientDirection);

    IntensityImage* surpressedImage = imageSurpressor(*sobelImage, refinedGradientDirection);

    IntensityImage* hystericImage = hysterisch(*surpressedImage, 5, 110);




    return hystericImage;
}

IntensityImage *StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
    IntensityImage *returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());;
    for (size_t i = 0; i < image.getHeight() * image.getWidth(); i++) {
        returnImage->setPixel(i, image.getPixel(i));
    }
    std::cout << "Klaar met onze kak <3" << '\n';
    return returnImage;
}
