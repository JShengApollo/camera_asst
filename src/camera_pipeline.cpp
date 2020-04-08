#include <iostream>

#ifdef __USE_HALIDE__
#include "Halide.h"
#include "halide_utils.hpp"
#endif

#include "camera_pipeline.hpp"

std::unique_ptr<Image<RgbPixel>> CameraPipeline::ProcessShot() const {
  // In this function you should implement your full RAW image processing pipeline.
  //   (1) Demosaicing
  //   (2) Address sensing defects such as bad pixels and image noise.
  //   (3) Apply local tone mapping based on the local laplacian filter or exposure fusion.
  //   (4) gamma correction
    
  // The starter code copies the raw data from the sensor to all rgb
  // channels. This results in a gray image that is just a
  // visualization of the sensor's contents.

  // BEGIN: CS348K STUDENTS MODIFY THIS CODE

  // put the lens cap on if you'd like to measure a "dark frame"
  sensor_->SetLensCap(false);
    
  // grab RAW pixel data from sensor
  const int width = sensor_->GetSensorWidth();
  const int height = sensor_->GetSensorHeight();
  auto raw_data = sensor_->GetSensorData(0, 0, width, height);
  auto raw_data_vec = sensor_->GetBurstSensorData(0, 0, width, height);

#ifdef __USE_HALIDE__
  std::cout << "Using Halide pipeline" << std::endl;

  Halide::Buffer<float> input =
    burstSensorDataToHalide(raw_data_vec);

  //Halide::Buffer<float> input =
    //sensorDataToHalide(raw_data.get(), width, height);

  std::cout << "Wrote Halide input" << std::endl;

  //// Halide pipeline that just copies 
  //// the raw data to the channels
  //Halide::Var x, y, c;
  //Halide::Func cameraPipeline;

  //// pixel data from the sensor is normalized to the 0-1 range, so
  //// scale by 255 for the final image output.  Output image pixels
  //// should be in the 0-255 range.
  //cameraPipeline(x, y, c) =
    //input(x, y) * 255.0f;

  //std::cout << "Realizing camera pipeline..." << std::endl;

  //Halide::Buffer<float> output =
    //cameraPipeline.realize(input.width(), input.height(), 3);

  //std::cout << "Got output" << std::endl;

  //std::unique_ptr<Image<RgbPixel>> image = rgbImageFromHalide(output);
  //return image;

#else
    
  std::cout << "Using vanilla C++ pipeline" << std::endl;
  // allocate 3-channel RGB output buffer to hold the results after processing 
  std::unique_ptr<Image<RgbPixel>> image(new Image<RgbPixel>(width, height));
  
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      const auto val = raw_data->data(row, col);
      auto& pixel = (*image)(row, col);

      // pixel data from the sensor is normalized to the 0-1 range, so
      // scale by 255 for the final image output.  Output image pixels
      // should be in the 0-255 range.
      pixel.r = val * 255.f;
      pixel.g = val * 255.f;
      pixel.b = val * 255.f;
    }
  }
  
  // return processed image output
  return image;
#endif

  // END: CS348K STUDENTS MODIFY THIS CODE  
}
