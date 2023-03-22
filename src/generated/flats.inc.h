const flat_t flats[] = {
	{ {0x16, 0x16, 0x67, 0x67, 0x19, 0x68, 0x79, 0x89, } },
	{ {0x16, 0x16, 0x17, 0x67, 0x77, 0x77, 0x78, 0x88, } },
	{ {0x16, 0x16, 0x17, 0x67, 0x18, 0x77, 0x78, 0x78, } },
	{ {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, } },
	{ {0x16, 0x16, 0x13, 0x13, 0x37, 0x37, 0x38, 0x39, } },
	{ {0x16, 0x16, 0x67, 0x19, 0x68, 0x79, 0x89, 0x89, } },
	{ {0x16, 0x16, 0x67, 0x67, 0x19, 0x68, 0x79, 0x79, } },
	{ {0x16, 0x16, 0x67, 0x67, 0x19, 0x68, 0x79, 0x79, } },
	{ {0x16, 0x16, 0x66, 0x67, 0x19, 0x68, 0x79, 0x79, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x67, 0x18, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x17, 0x67, 0x67, 0x67, } },
	{ {0x16, 0x16, 0x66, 0x66, 0x67, 0x19, 0x19, 0x69, } },
	{ {0x16, 0x16, 0x66, 0x66, 0x67, 0x19, 0x19, 0x69, } },
	{ {0x16, 0x16, 0x66, 0x66, 0x67, 0x19, 0x69, 0x69, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x66, 0x67, 0x67, 0x67, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x67, 0x67, 0x67, 0x19, } },
	{ {0x16, 0x16, 0x16, 0x13, 0x13, 0x13, 0x13, 0x36, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x67, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x17, 0x67, 0x67, } },
	{ {0x16, 0x16, 0x16, 0x66, 0x67, 0x67, 0x19, 0x19, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x66, 0x66, } },
	{ {0x16, 0x16, 0x16, 0x66, 0x67, 0x67, 0x19, 0x19, } },
	{ {0x16, 0x16, 0x16, 0x66, 0x66, 0x67, 0x19, 0x19, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x67, 0x67, 0x67, 0x67, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x67, 0x18, } },
	{ {0x16, 0x16, 0x16, 0x67, 0x67, 0x68, 0x68, 0x79, } },
	{ {0x16, 0x16, 0x16, 0x67, 0x67, 0x68, 0x68, 0x79, } },
	{ {0x16, 0x16, 0x16, 0x67, 0x67, 0x19, 0x68, 0x68, } },
	{ {0x16, 0x16, 0x16, 0x67, 0x67, 0x19, 0x68, 0x68, } },
	{ {0x16, 0x16, 0x66, 0x66, 0x19, 0x19, 0x69, 0x69, } },
	{ {0x16, 0x16, 0x66, 0x67, 0x19, 0x19, 0x68, 0x79, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x67, 0x18, } },
	{ {0x16, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, } },
	{ {0x16, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x66, 0x66, 0x67, 0x67, } },
	{ {0x16, 0x16, 0x16, 0x17, 0x17, 0x67, 0x18, 0x77, } },
	{ {0x16, 0x17, 0x67, 0x77, 0x78, 0x88, 0x88, 0x8b, } },
	{ {0x16, 0x16, 0x66, 0x66, 0x19, 0x19, 0x69, 0x69, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x66, 0x66, } },
	{ {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x12, } },
	{ {0x16, 0x16, 0x17, 0x18, 0x77, 0x78, 0x88, 0x88, } },
	{ {0x16, 0x16, 0x17, 0x18, 0x77, 0x78, 0x88, 0x88, } },
	{ {0x16, 0x11, 0x11, 0x17, 0x17, 0x17, 0x27, 0x27, } },
	{ {0x16, 0x16, 0x17, 0x18, 0x77, 0x78, 0x88, 0x88, } },
	{ {0x16, 0x16, 0x17, 0x67, 0x18, 0x77, 0x78, 0x78, } },
	{ {0x16, 0x16, 0x66, 0x67, 0x19, 0x19, 0x68, 0x79, } },
	{ {0x16, 0x16, 0x16, 0x17, 0x67, 0x18, 0x77, 0x78, } },
	{ {0x16, 0x16, 0x16, 0x17, 0x67, 0x18, 0x77, 0x78, } },
	{ {0x16, 0x16, 0x16, 0x17, 0x67, 0x18, 0x77, 0x78, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x66, 0x66, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x66, 0x66, 0x66, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x16, 0x66, 0x66, 0x66, } },
	{ {0x16, 0x16, 0x16, 0x16, 0x67, 0x67, 0x67, 0x19, } },
};