#include "GstAppSinkPipeline.h"
#include "GstreamerPipelines.h"

#include <boost/thread/lock_guard.hpp>
#include <iostream>

GstAppSinkPipeline::GstAppSinkPipeline()
: retrievedBuffer(0)
, currentBuffer(0)
{

}

GstAppSinkPipeline::~GstAppSinkPipeline()
{
	
}

void GstAppSinkPipeline::Initialize(std::string pipelineString)
{	
    //std::cout << pipelineString << std::endl;
	GstPipelineWrapper::InitializePipelineWithString(pipelineString);
	
	// setup appsink
	appsink = GstPipelineWrapper::GetElementByName(APPSINK_NAME);
	GstAppSinkCallbacks appsinkCallbacks;
	appsinkCallbacks.new_preroll	= &GstAppSinkPipeline::NewPrerollCallback;
	appsinkCallbacks.new_sample		= &GstAppSinkPipeline::NewSampleCallback;
	appsinkCallbacks.eos			= &GstAppSinkPipeline::EndOfStreamCallback; 
	
	gst_app_sink_set_drop			(GST_APP_SINK(appsink), true);
	gst_app_sink_set_max_buffers	(GST_APP_SINK(appsink), 1);
	//gst_app_sink_set_emit_signals	(GST_APP_SINK(appsink), true);
	gst_app_sink_set_callbacks		(GST_APP_SINK(appsink), &appsinkCallbacks, this, (GDestroyNotify)GstAppSinkPipeline::DestroyCallback);		
}

void GstAppSinkPipeline::EndOfStreamCallback(GstAppSink* appsink, gpointer user_data)
{

}

GstFlowReturn GstAppSinkPipeline::NewPrerollCallback(GstAppSink* appsink, gpointer user_data)
{
	GstSample* sample = gst_app_sink_pull_preroll(appsink);
	gst_sample_unref(sample);
	return GST_FLOW_OK;
}

GstFlowReturn GstAppSinkPipeline::NewSampleCallback(GstAppSink* appsink, gpointer user_data)
{
	//static double timer = glfwGetTime(); std::cout << "SAMPLE" << (glfwGetTime()-timer)*1000 << std::endl; timer = glfwGetTime();
	((GstAppSinkPipeline*) user_data)->ReceiveNewSample();
	return GST_FLOW_OK;

}
void GstAppSinkPipeline::DestroyCallback(gpointer user_data)
{
	std::cout << "DESTROY" << std::endl;
}

void GstAppSinkPipeline::ReceiveNewSample()
{	
	GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
	
	if (sample)
	{
		boost::lock_guard<boost::mutex> guard(bufferMutex);
		if (currentBuffer != 0) // release if not empty
		{
			//std::cout << "DROP!\n";
			gst_sample_unref(currentBuffer);
		}
		currentBuffer = sample;
	}
}

void GstAppSinkPipeline::ReleaseFrameBuffer()
{
	boost::lock_guard<boost::mutex> guard(bufferMutex);
	if (retrievedBuffer) gst_sample_unref(retrievedBuffer);
	retrievedBuffer = 0;
}

bool GstAppSinkPipeline::GetIsNewFrameAvailable()
{
	boost::lock_guard<boost::mutex> guard(bufferMutex);
	return ((retrievedBuffer == 0) && (currentBuffer != 0));
}

bool GstAppSinkPipeline::GetLatestFrameBuffer(void** frameBuffer)
{
	bool retrieving = false;
	
	boost::lock_guard<boost::mutex> guard(bufferMutex);
	if (retrievedBuffer == 0)
	{
		if (currentBuffer != 0)
		{
			retrievedBuffer = currentBuffer;
			currentBuffer = 0;
			retrieving = true;
		}
	}
	
	if (retrieving)
	{
		GstBuffer*		buffer;
		GstMapInfo 		map;

		buffer = gst_sample_get_buffer (retrievedBuffer);
		
		if (buffer)
		{
			gst_buffer_map (buffer, &map, GST_MAP_READ);
			(*frameBuffer) = map.data;
			gst_buffer_unmap (buffer, &map);
		}
		else return false;
	}
	
	return true;
}

bool GstAppSinkPipeline::GetResolution(int* width, int* height)
{
    GstCaps *caps;
    GstStructure *s;
    *width = 0;
    *height = 0;

    boost::lock_guard<boost::mutex> guard(bufferMutex);
    if (currentBuffer != 0) {
        caps = gst_sample_get_caps(currentBuffer);
        s = gst_caps_get_structure(caps, 0);
        gst_structure_get_int(s, "width", width);
        gst_structure_get_int(s, "height", height);
        // printf("resolution: %dx%d\n", *width, *height);
        return true;
    }
    return false;
}
