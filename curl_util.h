#pragma once

#include <iostream>
#include <string>

#include <curl/curl.h>

std::string urlEncode(const std::string &input);
std::string apiCall(CURL* hnd, std::string urlSuffix);