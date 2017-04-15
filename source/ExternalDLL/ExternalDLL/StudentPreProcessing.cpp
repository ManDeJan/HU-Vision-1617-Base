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
    int    radius = 1;
    double sigma = 0.1;

    // Sobel kernels:
    kernel_type sobel_X = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
    kernel_type sobel_Y = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };

    // Deze kernel doet niks, het is een fopkernel:
    kernel_type fopKernel = { { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0, 0 } };

    // Deze kernel wordt in de default implemenatie gebruikt:
    kernel_type mega_karnel =
    { { 0, 0, 0, 1, 1, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 1, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 1, 1, 0, 0, 0 },
      { 1, 1, 1, -4, -4, -4, 1, 1, 1 },
      { 1, 1, 1, -4, -4, -4, 1, 1, 1 },
      { 1, 1, 1, -4, -4, -4, 1, 1, 1 },
      { 0, 0, 0, 1, 1, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 1, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 1, 1, 0, 0, 0 } };


    // Lekker dialiseren doe je met deze kernel:
    kernel_type dilation_kernol =
    { { 0, 1, 0 },
      { 1, 1, 1 },
      { 0, 1, 0 } };


    // Haha uitgecommente statements:
    // IntensityImage *karnelImage = kernelApplyer(image, { mega_karnel });
    // return karnelImage;

    // Laplacian gebruiken we niet :)
    kernel_type laplacian = { { 0.5, 1, 0.5 }, { 1, -6, 1 }, { 0.5, 1, 0.5 } };

    // Blurren met deze kernel, gaat best wel prima!
    kernel_type gausKernel = produce2dGaussianKernel(radius, sigma);

    // Hier blurren we :)
    IntensityImage *blurImage = kernelApplyer(image, { gausKernel });

    // Dit is een lambda die je aan de kernelApplyer meegeeft met de 2 sobel kernels om een goeie sobel merge te doen
    auto sobel_merge = [](std::vector<int> pekkels) { return sqrt(pow(pekkels[0], 2) + pow(pekkels[1], 2)); };

    // Deze merge doet niks!
    auto fop_merge = [](std::vector<int> pekkels) { return pekkels[1]; };

    // Hier slaan we richtingen in op
    gradientImage   gradientDirection;
    // Even de sobel toepassen:
    IntensityImage *sobelImage = kernelApplyer(*blurImage, { sobel_X, sobel_Y }, sobel_merge, &gradientDirection);


    // Deze code gebruiken we niet, haha.
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

    // maakt van heel veel richtingen heel weinig richtingen!
    refinedGradientImage refinedGradientDirection = gradientRefiner(gradientDirection);

    // non-Max surpressie!
    IntensityImage *surpressedImage = imageSurpressor(*sobelImage, refinedGradientDirection);

    // WAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH
    IntensityImage *hystericImage = hysterisch(*surpressedImage, 20, 170);

    // Helpt dilation? Lees nu het meetrapport!
    IntensityImage *returnImage = kernelApplyer(*hystericImage, { dilation_kernol });

    return returnImage;
}

IntensityImage *StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
    IntensityImage *returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
    // Hier inverten we de image, want thresholding deden we hierboven al.
    for (int i = 0; i < image.getHeight() * image.getWidth(); i++) {
        returnImage->setPixel(i, 255 - image.getPixel(i));
    }

    std::cout << "Klaar met onze kak <3" << '\n';
    return returnImage;
}
