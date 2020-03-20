/*
 * Copyright (c) 2020 Karsten Becker All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

#include <string>
#include <utility>
#include <vector>

#include "SHICommunicator.h"
#include "SHISensor.h"

namespace SHI {

class OpenhabRestCommunicatorConfig : public Configuration {
 public:
  OpenhabRestCommunicatorConfig() {}
  explicit OpenhabRestCommunicatorConfig(const JsonObject &obj);
  void fillData(JsonObject &doc) const override;
  int CONNECT_TIMEOUT = 500;
  int DATA_TIMEOUT = 1000;
  std::string hierachySeperator = "_";
  std::string baseURL = "http://192.168.188.250:8080";
  std::string namePrefix = "s_ESP32_";
  int statusRetryCount = 15;

 protected:
  int getExpectedCapacity() const override;
};

class OpenhabRestCommunicator : public Communicator {
 public:
  explicit OpenhabRestCommunicator(const OpenhabRestCommunicatorConfig &config)
      : Communicator("OpenhabREST"), config(config) {}
  void setupCommunication() override {}
  void loopCommunication() override {}
  void newReading(const MeasurementBundle &reading) override;
  void newStatus(const Measurement &status, SHIObject *src) override;
  std::vector<std::pair<std::string, std::string>> getStatistics() override;
  const Configuration *getConfig() const override { return &config; }
  bool reconfigure(Configuration *newConfig) {
    config = castConfig<OpenhabRestCommunicatorConfig>(newConfig);
    return true;
  }

 private:
  int errorCount = 0, httpErrorCount = 0, httpCount = 0,
      missedConnectionCount = 0;
  String prefix;
  void uploadInfo(const std::string &item, const std::string &value,
                  bool tryHard = false);
  void printError(HTTPClient *http, int httpCode);
  OpenhabRestCommunicatorConfig config;
};

}  // namespace SHI
