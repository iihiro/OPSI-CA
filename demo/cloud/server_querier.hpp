/*
 * Copyright 2018 Yamana Laboratory, Waseda University
 * Supported by JST CREST Grant Number JPMJCR1503, Japan.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE‐2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SERVER_QUERIER_HPP
#define SERVER_QUERIER_HPP

#include <stdsc/stdsc_callback_function_container.hpp>
#include <stdsc/stdsc_callback_function.hpp>
#include <stdsc/stdsc_state.hpp>
#include <stdsc/stdsc_packet.hpp>
#include <opsica_share/opsica_packet.hpp>
#include <opsica_cloud/opsica_cloud_callback_function.hpp>
#include <opsica_cloud/opsica_cloud_callback_param.hpp>
#include <opsica_cloud/opsica_cloud_thread.hpp>
#include <share/define.hpp>

namespace opsica_demo
{

struct ServerQuerier
{
    ServerQuerier(opcl::CallbackParam& param, stdsc::StateContext& state,
                  const char* port = CLOUD_PORT_FOR_QUERIER)
    {
        stdsc::CallbackFunctionContainer callback;

        std::shared_ptr<stdsc::CallbackFunction> cb_conn(
          new opcl::CallbackFunctionRequestConnect(param));
        callback.set(opsh::kControlCodeRequestConnect, cb_conn);

        std::shared_ptr<stdsc::CallbackFunction> cb_disconn(
          new opcl::CallbackFunctionRequestDisconnect(param));
        callback.set(opsh::kControlCodeRequestDisconnect, cb_disconn);

        std::shared_ptr<stdsc::CallbackFunction> cb_pubkey(
          new opcl::CallbackFunctionDataPubkey(param));
        callback.set(opsh::kControlCodeDataPubKey, cb_pubkey);

        std::shared_ptr<stdsc::CallbackFunction> cb_compute(
          new opcl::CallbackFunctionDataCompute(param));
        callback.set(opsh::kControlCodeDataCompute, cb_compute);

        std::shared_ptr<stdsc::CallbackFunction> cb_result(
          new opcl::CallbackFunctionDownloadResult(param));
        callback.set(opsh::kControlCodeDownloadResult, cb_result);

        cloud_ = std::make_shared<opcl::CloudThread>(port, callback, state);
    }

    ~ServerQuerier(void) = default;

    void start(void)
    {
        cloud_->start();
    }

    void join(void)
    {
        cloud_->join();
    }

private:
    std::shared_ptr<opcl::CloudThread> cloud_;
};

} /* namespace opsica_demo */

#endif /* SERVER_QUERIER_HPP */
