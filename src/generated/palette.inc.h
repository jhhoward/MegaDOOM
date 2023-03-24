#ifdef WIN32
const uint32_t gamePalette[] = {
	0xff000000, 0xff240000, 0xffda0000, 0xff0000b6, 0xffda00da, 0xff0000ff, 0xff002424, 0xff484848, 0xff6d6d6d, 0xff246d91, 0xff486dff, 0xffffb6b6, 0xffb6daff, 0xff6dff6d, 0xff24ffff, 0xffffffff
};
#else
const uint16_t gamePalette[] = {
	0x0, 0x200, 0xe00, 0xc, 0xe0e, 0xe, 0x22, 0x444, 0x666, 0x26a, 0x46e, 0xecc, 0xcee, 0x6e6, 0x2ee, 0xeee
};
#endif