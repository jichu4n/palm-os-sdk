#include "starter.rh"

resource 'tFRM' (1000, "Main") {
	0,
	0,
	240,
	240,
	usable,
	notModal,
	noSaveBehind,
	1000,
	0,
	1000,
	0,
	{	/* array ObjectArray: 1 elements */
		/* [1] */
		1000, "tTTL"
	}
};



data 'tTTL' (1000) {
	$"4578 616d 706c 6558 00"                                                              
};

