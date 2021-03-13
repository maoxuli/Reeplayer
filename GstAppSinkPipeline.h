#pragma once

#include <gst/app/gstappsink.h>
#include "GstPipelineWrapper.h"

class GstAppSinkPipeline : public GstPipelineWrapper
{
public:
    GstAppSinkPipeline();
	~GstAppSinkPipeline();

	void Initialize(std::string pipelineString);

	bool GetIsNewFrameAvailable();
	bool GetLatestFrameBuffer(void** frameBuffer);
	void ReleaseFrameBuffer();

    bool GetResolution(int* width, int* height);

private:
	GstElement* 	appsink;

	boost::mutex	bufferMutex;
	void			ReceiveNewSample();
	GstSample*		retrievedBuffer;
	GstSample*		currentBuffer;

	static void				DestroyCallback		(gpointer data);
	static void				EndOfStreamCallback	(GstAppSink* appsink, gpointer user_data);
	static GstFlowReturn	NewPrerollCallback	(GstAppSink* appsink, gpointer user_data);
	static GstFlowReturn	NewSampleCallback	(GstAppSink* appsink, gpointer user_data);
};
