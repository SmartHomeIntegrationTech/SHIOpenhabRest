/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

// WARNING, this is an automatically generated file!
// Don't change anything in here.
// Last update 2020-03-25

# include <iostream>
# include <string>


# include "SHIOpenhabRestCommunicator.h"
// Configuration implementation for class SHI::OpenhabRestCommunicatorConfig

namespace {
    
}  // namespace

SHI::OpenhabRestCommunicatorConfig::OpenhabRestCommunicatorConfig(const JsonObject &obj):
      CONNECT_TIMEOUT(obj["CONNECT_TIMEOUT"] | 500),
      DATA_TIMEOUT(obj["DATA_TIMEOUT"] | 1000),
      hierachySeperator(obj["hierachySeperator"] | "_"),
      baseURL(obj["baseURL"] | "http://192.168.188.250:8080"),
      namePrefix(obj["namePrefix"] | "s_ESP32_"),
      statusRetryCount(obj["statusRetryCount"] | 15)
  {}

void SHI::OpenhabRestCommunicatorConfig::fillData(JsonObject &doc) const {
    doc["CONNECT_TIMEOUT"] = CONNECT_TIMEOUT;
  doc["DATA_TIMEOUT"] = DATA_TIMEOUT;
  doc["hierachySeperator"] = hierachySeperator;
  doc["baseURL"] = baseURL;
  doc["namePrefix"] = namePrefix;
  doc["statusRetryCount"] = statusRetryCount;
}

int SHI::OpenhabRestCommunicatorConfig::getExpectedCapacity() const {
  return JSON_OBJECT_SIZE(6);
}

