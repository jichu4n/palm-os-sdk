#include <UIResDefs.r>

resource 'tFRM' (4300, "pleasewait") {
	2,
	130,
	156,
	28,
	usable,
	modal,
	saveBehind,
	4300,
	0,
	0,
	0,
	{	/* array ObjectArray: 1 elements */
		/* [1] */
		4301, "tLBL"
	}
};

resource 'tLBL' (4301, "text") {
	4301,
	28,
	10,
	usable,
	boldFont,
	"Please wait..."
};

resource 'tFRM' (9500, "View") {
	2,
	2,
	156,
	156,
	usable,
	modal,
	saveBehind,
	9500,
	0,
	0,
	0,
	{	/* array ObjectArray: 4 elements */
		/* [1] */
		9500, "tTTL",
		/* [2] */
		9501, "tFLD",
		/* [3] */
		9502, "tBTN",
		/* [4] */
		9503, "tSCL"

	}
};

resource 'tTTL' (9500) {
	"Text Viewer"
};

resource 'tFLD' (9501, "Body") {
	9501,
	3,
	16,
	142,
	120,
	usable,
	notEditable,
	underlined,
	notSingleLine,
	notDynamicSize,
	leftJustified,
	10240,
	stdFont,
	notAutoShift,
	hasScrollbar,
	notNumeric
};

resource 'tBTN' (9502, "Done") {
	9502,
	4,
	139,
	25,
	12,
	usable,
	leftAnchor,
	frame,
	nonBoldFrame,
	stdFont,
	"Done"
};

resource 'tSCL' (9503, "scrollbar") {
	9503,
	146,
	17,
	7,
	120,
	usable,
	0,
	0,
	0,
	0
};

resource 'Talt' (1001) {
	confirmationAlert,
	0, // help Rsc ID
	1, // # Buttons
	0, // Default Button Index
	 "VMTextPlugin Alert",
	 "VMTextPlugin called.",
	{
		 "OK",
	}
};