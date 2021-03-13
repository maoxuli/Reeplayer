#pragma once

#include <sstream>

#define LINK			" ! "
#define NEW_LINKAGE		" "
#define PAD				"."

#define APPSINK_NAME	"appsink_name"

static std::string CreateRtspSinkPipeline(const std::string& url)
{
    std::stringstream pipelineString;

    pipelineString
        << "rtspsrc location=" << url
        << LINK
        << "rtph264depay"
        << LINK
        << "h264parse"
        << LINK
        << "queue"
        << LINK
        << "avdec_h264"
        << LINK
        << "videoconvert"
        << LINK
        << "video/x-raw,format=(string)BGR"
        << LINK
        << "appsink name=" << APPSINK_NAME;

    return pipelineString.str();
}
