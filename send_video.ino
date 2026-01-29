#include <string.h>

// Camera: https://github.com/RuiSantosdotme/arduino-esp32-CameraWebServer/blob/master/CameraWebServer/CameraWebServer.ino
  #include "esp_camera.h"

  // Data structure
  static camera_config_t camera_config = {
    // Power/reset operations
    .pin_pwdn = 32,                      // Power down pin
    .pin_reset = -1,                     // Reset is performed in software

    // Pins that handle config messages
    .pin_sccb_sda = 26,                  // Sends the messages
    .pin_sccb_scl = 27,                  // Clock pin used to synchronize message timing

    // Highway of pins for sending the image
      // Each digit of an 8-bit word, 1 pulse at a time
      .pin_d7        = 35,
      .pin_d6        = 34,
      .pin_d5        = 39,
      .pin_d4        = 36,
      .pin_d3        = 21,
      .pin_d2        = 19,
      .pin_d1        = 18,
      .pin_d0        = 5,

      // Click pins
      .pin_vsync     = 25,               // Marks the start of an image
      .pin_href      = 23,               // On high while a row is being sent
      .pin_pclk      = 22,               // On high when a new word should be read

    // Camera's internal clock
    .pin_xclk = 0,                       // Pin

    .xclk_freq_hz = 20000000             // Frequency
    
    .ledc_timer = LEDC_TIMER_0,          // Internal clock mechanism
    .ledc_channel = LEDC_CHANNEL_0,      // Internal clock output

    // Image config
    .pixel_format = PIXFORMAT_JPEG,      // Image format
    .frame_size = FRAMESIZE_SVGA,        // Resolution
    .jpeg_quality = 15,                  // Compression level (lower number = higher quality)
    .fb_count = 1,                       // Number of frame buffers
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY  // Only write into a buffer when its empty
  }

void setup() {
  // put your setup code here, to run once:
  //code
  //more code
}

void loop() {
  // put your main code here, to run repeatedly:

}
