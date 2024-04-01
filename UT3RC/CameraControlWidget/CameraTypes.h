#pragma once

#include <vector>
#include <string>

typedef enum {
    k_ADSIMDETECTOR_1,
    k_ADSIMDETECTOR_2,
    k_NOCAMERA
} CameraIndex_t;

static std::vector<std::string> CameraNames = {"13SIM1:cam1", "13SIM1:cam2"};
static std::vector<std::string> ImageSources = {"13SIM1:Pva1:Image", "13SIM1:Pva2:Image"};