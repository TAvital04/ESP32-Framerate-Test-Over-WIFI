#include <string.h>

// Camera: https://github.com/RuiSantosdotme/arduino-esp32-CameraWebServer/blob/master/CameraWebServer/CameraWebServer.ino
  #include "esp_camera.h"

  // Global variables
  camera_fb_t *image_buffer = nullptr;

  // Data structure
  static camera_config_t camera_config = {
    // Power/reset operations
    .pin_pwdn        = 32,                     // Power down pin
    .pin_reset       = -1,                     // Reset is performed in software
    .pin_xclk        = 0,                      // Camera clock hardware

    // Pins that handle config messages
    .pin_sccb_sda    = 26,                     // Sends the messages
    .pin_sccb_scl    = 27,                     // Clock pin used to synchronize message timing

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
      .pin_vsync     = 25,                     // Marks the start of an image
      .pin_href      = 23,                     // On high while a row is being sent
      .pin_pclk      = 22,                     // On high when a new word should be read

    // Camera's internal clock
    .xclk_freq_hz    = 20000000,               // Frequency
    
    .ledc_timer      = LEDC_TIMER_0,           // Internal clock mechanism
    .ledc_channel    = LEDC_CHANNEL_0,         // Internal clock output

    // Image config
    .pixel_format    = PIXFORMAT_JPEG,         // Image format
    .frame_size      = FRAMESIZE_SVGA,         // Resolution
    .jpeg_quality    = 15,                     // Compression level (lower number = higher quality)
    .fb_count        = 1,                      // Number of frame buffers
    .grab_mode       = CAMERA_GRAB_WHEN_EMPTY  // Only write into a buffer when its empty
  };

// HTTP Request
  #include <WiFi.h>
  #include <WebServer.h>

  #include <time.h>

                                           /* CHANGE THIS */
  /*********************************************************/
  
    static const char *WIFI_SSID = "Tals Phone";
    static const char *WIFI_PASS = "asdfghjk";

  /**********************************************************/

  // Global variables
  WebServer server(8080);

  int code;
  char *response_cstr;

  time_t timer = time(NULL);

  int count = 0;

  // Functions
  void send_response() {
    count += 1;
    String html_body = "";

    // html_body += (const char*)image_buffer->buf;
    // html_body += "     ";
    // html_body += difftime(timer, time(NULL));
    // html_body += "     ";

    html_body += count;
    html_body += "     ";

    server.send(200, "text/html", html_body);

    printf("Sent\n");
  }

void setup() {
  Serial.begin(115200);
  delay(300);

  // Initialize the camera
  Serial.printf("Initializing the camera.\n");
  esp_err_t status = esp_camera_init(&camera_config);

  if(status != ESP_OK) {
    Serial.printf("Failed to initialize camera: 0x%x.\n");
    return;
  }

  // Connect to the WIFI
  Serial.printf("Connecting to the WiFi\n");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.printf("\nWIFI connected at %s\n", WiFi.localIP().toString().c_str());
  server.on("/", send_response);
  server.begin();

  Serial.printf("Setup complete, taking video.\n");
}

void loop() {
  delay(1);

  while(true) {
    // Contact the server
    server.handleClient();

    // Take a picture, handle errors
    image_buffer = esp_camera_fb_get();

    if(image_buffer == NULL) {
      Serial.printf("Failed to take a picture.\n");
      return;
    }

    // Send via WIFI
    send_response();

    // Refresh the buffer
    esp_camera_fb_return(image_buffer);
    delay(1000);
  }

  // Serial.printf("Program terminated successfully.");
}
