#include <ladspa.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* functions */
    LADSPA_Handle haas_instantiate(const struct _LADSPA_Descriptor*, 
	    unsigned long sr);
    void haas_connect_port(LADSPA_Handle, unsigned long port, LADSPA_Data*);
    void haas_activate(LADSPA_Handle);
    void haas_run(LADSPA_Handle, unsigned long sample_count);
    void haas_run_adding(LADSPA_Handle, unsigned long sample_count);
    void haas_set_run_adding_gain(LADSPA_Handle, LADSPA_Data);
    void haas_deactivate(LADSPA_Handle);
    void haas_cleanup(LADSPA_Handle);

    /* descriptor */
    static LADSPA_Descriptor descr = {
	0x42,		// UniqueID TODO
	"haas",		// Label
	0,		// Properties TODO
	"Haas Effect",	// Name
	"Hans Fugal",	// Maker
	"Copyright (C) 2005 Hans Fugal <hans@fugal.net>", // Copyright
	4,		// PortCount
	0,		// PortDescriptors TODO
	0,		// PortNames TODO
	0,		// PortRangeHints TODO
	0,		// ImplementationData TODO
	haas_instantiate,	// instantiate
	haas_connect_port,	// connect_port
	0,		// activate XXX
	haas_run,	// run
	0,		// run_adding XXX
	0,		// set_run_adding_gain XXX
	0,		// deactivate XXX
	haas_cleanup	// cleanup
    };

#ifdef __cplusplus
}
#endif
