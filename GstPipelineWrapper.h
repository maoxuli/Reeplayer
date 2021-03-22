#pragma once

#include <string>
#include <thread>
#include <gst/gst.h>

class GstPipelineWrapper
{
public:
    GstPipelineWrapper(void);
    virtual ~GstPipelineWrapper();

    void InitializePipelineWithString(std::string pipelineString);
    GstElement* GetElementByName(std::string element_name);

    bool SetPipelineState(GstState state);
    GstState GetPipelineState();

    void WaitForEOS();

    void set_is_verbose(bool is_verbose);
    bool get_is_verbose() const;

protected:
    bool is_verbose_; // default: false

    GstElement* pipeline_;
    GstBus* bus_;
    GMainLoop* g_main_loop_;

    std::thread g_main_loop_thread_;
    void RunningMainLoop();

    void FreePipeline();

    static bool GstMessageParser(GstBus* bus, GstMessage* msg, GstPipelineWrapper* pipeline);
    static void EnsureGstreamerInitialization();
};
