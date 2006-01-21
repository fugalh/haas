#include <ladspa.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* functions */
    static LADSPA_Descriptor descr = {
	0x42,		// UniqueID
	"haas",		// Label
	0,		// Properties
	"Haas Effect",	// Name
	"Hans Fugal",	// Maker
	"Copyright (C) 2005 Hans Fugal <hans@fugal.net>", // Copyright
	4,		// PortCount
	0,		// PortDescriptors
	0,		// PortNames
	0,		// PortRangeHints
	0,		// ImplementationData
	haas_instantiate,	// instantiate
	haas_connect_port,	// connect_port
	0,		// activate
	haas_run,	// run
	0,		// run_adding
	0,		// set_run_adding_gain
	0,		// deactivate
	haas_cleanup	// cleanup
    };

#ifdef __cplusplus
}
#endif
