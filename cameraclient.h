/**
 * This file is generated by jsonrpcstub, DO NOT CHANGE IT MANUALLY!
 */

#ifndef JSONRPC_CPP_STUB_CAMERACLIENT_H_
#define JSONRPC_CPP_STUB_CAMERACLIENT_H_

#include <jsonrpccpp/client.h>

class CameraClient : public jsonrpc::Client
{
    public:
        CameraClient(jsonrpc::IClientConnector &conn, jsonrpc::clientVersion_t type = jsonrpc::JSONRPC_CLIENT_V2) : jsonrpc::Client(conn, type) {}

        bool restart(const std::string& password) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["password"] = password;
            Json::Value result = this->CallMethod("restart",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool shutdown(const std::string& password) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["password"] = password;
            Json::Value result = this->CallMethod("shutdown",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value checkSystemState() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("checkSystemState",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value checkFolders() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("checkFolders",p);
            if (result.isArray())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool createFolder(const std::string& path) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["path"] = path;
            Json::Value result = this->CallMethod("createFolder",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool changeFolder(const std::string& path) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["path"] = path;
            Json::Value result = this->CallMethod("changeFolder",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        std::string checkCurrentFolder() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("checkCurrentFolder",p);
            if (result.isString())
                return result.asString();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value checkFiles(const std::string& path) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["path"] = path;
            Json::Value result = this->CallMethod("checkFiles",p);
            if (result.isArray())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool changeLiveMode(int mode) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["mode"] = mode;
            Json::Value result = this->CallMethod("changeLiveMode",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value checkLiveState() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("checkLiveState",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool startCalib() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("startCalib",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool pauseCalib() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("pauseCalib",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool resetCalib() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("resetCalib",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool saveCalib() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("saveCalib",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool startStreaming() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("startStreaming",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool stopStreaming() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("stopStreaming",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool startRecording() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("startRecording",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool stopRecording() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("stopRecording",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value checkUploadState() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("checkUploadState",p);
            if (result.isObject())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool startUploading() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("startUploading",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool stopUploading() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("stopUploading",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        bool setFieldCorners(const Json::Value& corners) throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p["corners"] = corners;
            Json::Value result = this->CallMethod("setFieldCorners",p);
            if (result.isBool())
                return result.asBool();
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
        Json::Value checkFieldCorners() throw (jsonrpc::JsonRpcException)
        {
            Json::Value p;
            p = Json::nullValue;
            Json::Value result = this->CallMethod("checkFieldCorners",p);
            if (result.isArray())
                return result;
            else
                throw jsonrpc::JsonRpcException(jsonrpc::Errors::ERROR_CLIENT_INVALID_RESPONSE, result.toStyledString());
        }
};

#endif //JSONRPC_CPP_STUB_CAMERACLIENT_H_
