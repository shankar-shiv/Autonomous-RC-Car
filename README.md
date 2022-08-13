## Self Driving Car

See self-driving in action  

<a href="https://youtu.be/spPVM_PNJ7A" target="_blank"><img src="/preview.jpg" width="360" height="240"/></a>

This project builds a self-driving RC car using Raspberry Pi, ESP8266 and OpenCV. Raspberry Pi collects video stream data from Pi Camera and sends data to a computer using socket programming. The computer processes the video images for Neural Network Inferencing. A Neural Network model runs on computer and makes predictions for steering based on the input images. Predictions are then sent to the ESP8266 via a POST request, received by the ESP8266 (web server) via a GET request. 

TODO
- Traffic Light Detection
- Distance Measurement
- Collision Avoidance

More information to be added soon ...