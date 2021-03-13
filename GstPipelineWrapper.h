#pragma once

#include <string>
#include <gst/gst.h>
#include <boost/thread.hpp>

class GstPipelineWrapper
{
public:
	GstPipelineWrapper(void);
	~GstPipelineWrapper(void);

	void		InitializePipelineWithString(std::string pipelineString);
	GstElement* GetElementByName(std::string element_name);

	void		set_is_verbose(bool is_verbose);
	bool		get_is_verbose() const ; 

	bool		SetPipelineState(GstState state);
	GstState	GetPipelineState();

private:
	bool			is_verbose_; // default: false

	GstElement*		pipeline_;
	GstBus*			bus_;

	boost::thread	g_main_loop_thread_;
	void			RunningMainLoop();

	void			FreePipeline();

	static bool 	GstMessageParser(GstBus* bus, GstMessage* msg, GstPipelineWrapper* pipeline);
	static void		EnsureGstreamerInitialization();

};
