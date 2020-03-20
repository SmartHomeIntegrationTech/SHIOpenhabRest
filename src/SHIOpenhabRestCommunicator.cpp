/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#include "SHIOpenhabRestCommunicator.h"

#include <Arduino.h>
#include <HTTPClient.h>

#include <string>

#include "SHIHardware.h"
#include "SHISensor.h"

void SHI::OpenhabRestCommunicator::newReading(
    const SHI::MeasurementBundle &reading) {
  if (!isConnected) {
    missedConnectionCount++;
    return;
  }
  SHI::hw->feedWatchdog();
  for (auto &&data : reading.data) {
    if (data.getDataState() == SHI::MeasurementDataState::VALID) {
      uploadInfo(data.getMetaData()->getQualifiedName(config.hierachySeperator),
                 data.stringRepresentation);
      SHI::hw->feedWatchdog();
    }
  }
}

void SHI::OpenhabRestCommunicator::newStatus(const SHI::Measurement &status,
                                             SHIObject *src) {
  if (!isConnected) {
    SHI_LOGINFO(("Not uploading: " +
                 String(status.getMetaData()->getQualifiedName().c_str()) +
                 " as currently not connected")
                    .c_str());
    missedConnectionCount++;
    return;
  }
  uploadInfo(status.getMetaData()->getQualifiedName(config.hierachySeperator),
             status.stringRepresentation, true);
}

void SHI::OpenhabRestCommunicator::uploadInfo(const std::string &item,
                                              const std::string &value,
                                              bool tryHard) {
  SHI_LOGINFO((item + " " + value).c_str());
  int retryCount = 0;
  do {
    HTTPClient http;
    http.begin(String(config.baseURL.c_str()) + "/rest/items/" +
               config.namePrefix.c_str() + item.c_str() + "/state");
    http.setConnectTimeout(config.CONNECT_TIMEOUT);
    http.setTimeout(config.DATA_TIMEOUT);
    int httpCode = http.PUT(String(value.c_str()));
    printError(&http, httpCode);
    http.end();
    if (httpCode >= 200 && httpCode < 300)
      return;  // Either return early or try until success
    retryCount++;
    SHI::hw->feedWatchdog();
  } while (tryHard && retryCount < config.statusRetryCount);
}

void SHI::OpenhabRestCommunicator::printError(HTTPClient *http, int httpCode) {
  // httpCode will be negative on error
  if (httpCode > 0) {
    if (httpCode < 200 || httpCode > 299) {
      httpErrorCount++;
      SHI_LOGWARN(("response:" + String(httpCode, 10)).c_str());
    }
    httpCount++;
  } else {
    errorCount++;
    SHI_LOGINFO(("Failed " + String(httpCode, 10)).c_str());
    statusMessage =
        ("Last transmit failed with " + String(httpCode, 10)).c_str();
  }
}

std::vector<std::pair<std::string, std::string>>
SHI::OpenhabRestCommunicator::getStatistics() {
  return {{"httpFatalErrorCount", String(errorCount).c_str()},
          {"httpErrorCount", String(httpErrorCount).c_str()},
          {"missedConnectionCount", String(missedConnectionCount).c_str()},
          {"httpCount", String(httpCount).c_str()}};
}
