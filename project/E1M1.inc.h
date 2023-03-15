// Level data for E1M1
vertex_t E1M1_vertices[] = {
	{-352,256},
	{256,128},
	{256,-416},
	{-352,-416},
	{-32,256},
	{-32,128},
	{-352,352},
	{-32,352},
	{-32,-416},
};
sector_t E1M1_sectors[] = {
	{ 0, 128, 1, 1, 192, 0, 0 },
	{ 32, 96, 1, 1, 192, 0, 0 },
};
subsector_t E1M1_subsectors[] = {
	{ 4, 0 },
	{ 3, 4 },
	{ 4, 7 },
};
node_t E1M1_nodes[] = {
	{ -32, 256, 0, -128, { { 256, -416, -352, -32, }, { 128, -416, -32, 256, }, }, {32768, 32769}},
	{ -352, 256, 320, 0, { { 256, -416, -352, 256, }, { 352, 256, -352, -32, }, }, {0, 32770}},
};
line_t E1M1_lines[] = {
	{ &E1M1_vertices[0], &E1M1_vertices[6], 0, 96, 1, 0, 0, {0, -1}, &E1M1_sectors[1], 0},
	{ &E1M1_vertices[1], &E1M1_vertices[2], 0, -544, 1, 0, 0, {1, -1}, &E1M1_sectors[0], 0},
	{ &E1M1_vertices[2], &E1M1_vertices[3], -608, 0, 1, 0, 0, {2, -1}, &E1M1_sectors[0], 0},
	{ &E1M1_vertices[3], &E1M1_vertices[0], 0, 672, 1, 0, 0, {3, -1}, &E1M1_sectors[0], 0},
	{ &E1M1_vertices[4], &E1M1_vertices[5], 0, -128, 1, 0, 0, {4, -1}, &E1M1_sectors[0], 0},
	{ &E1M1_vertices[5], &E1M1_vertices[1], 288, 0, 1, 0, 0, {5, -1}, &E1M1_sectors[0], 0},
	{ &E1M1_vertices[6], &E1M1_vertices[7], 320, 0, 1, 0, 0, {6, -1}, &E1M1_sectors[1], 0},
	{ &E1M1_vertices[7], &E1M1_vertices[4], 0, -96, 1, 0, 0, {7, -1}, &E1M1_sectors[1], 0},
	{ &E1M1_vertices[0], &E1M1_vertices[4], 320, 0, 4, 0, 0, {8, 9}, &E1M1_sectors[0], &E1M1_sectors[1]},
};
side_t E1M1_sides[] = {
	{ 0, 0, 0, 0, 1, &E1M1_sectors[1], },
	{ 0, 0, 0, 0, 1, &E1M1_sectors[0], },
	{ 0, 0, 0, 0, 1, &E1M1_sectors[0], },
	{ 0, 0, 0, 0, 1, &E1M1_sectors[0], },
	{ 320, 0, 0, 0, 1, &E1M1_sectors[0], },
	{ 493, 0, 0, 0, 1, &E1M1_sectors[0], },
	{ 0, 0, 0, 0, 1, &E1M1_sectors[1], },
	{ 0, 0, 0, 0, 1, &E1M1_sectors[1], },
	{ 0, 0, 1, 1, 0, &E1M1_sectors[0], },
	{ 0, 0, 0, 0, 0, &E1M1_sectors[1], },
};
seg_t E1M1_segs[] = {
	{ &E1M1_vertices[8], &E1M1_vertices[3], 288, -32768, &E1M1_sides[2], &E1M1_lines[2]},
	{ &E1M1_vertices[3], &E1M1_vertices[0], 0, 16384, &E1M1_sides[3], &E1M1_lines[3]},
	{ &E1M1_vertices[4], &E1M1_vertices[5], 0, -16384, &E1M1_sides[4], &E1M1_lines[4]},
	{ &E1M1_vertices[0], &E1M1_vertices[4], 0, 0, &E1M1_sides[8], &E1M1_lines[8]},
	{ &E1M1_vertices[1], &E1M1_vertices[2], 0, -16384, &E1M1_sides[1], &E1M1_lines[1]},
	{ &E1M1_vertices[2], &E1M1_vertices[8], 0, -32768, &E1M1_sides[2], &E1M1_lines[2]},
	{ &E1M1_vertices[5], &E1M1_vertices[1], 0, 0, &E1M1_sides[5], &E1M1_lines[5]},
	{ &E1M1_vertices[0], &E1M1_vertices[6], 0, 16384, &E1M1_sides[0], &E1M1_lines[0]},
	{ &E1M1_vertices[6], &E1M1_vertices[7], 0, 0, &E1M1_sides[6], &E1M1_lines[6]},
	{ &E1M1_vertices[7], &E1M1_vertices[4], 0, -16384, &E1M1_sides[7], &E1M1_lines[7]},
	{ &E1M1_vertices[4], &E1M1_vertices[0], 0, -32768, &E1M1_sides[9], &E1M1_lines[8]},

};
mapthing_t E1M1_things[] = {
	{ -160, -32, 270, 1, 7},

};
map_t map_E1M1 = {
	E1M1_vertices,
	E1M1_sides,
	E1M1_nodes,
	E1M1_subsectors,
	E1M1_segs,
	E1M1_lines,
	E1M1_sectors,
	E1M1_things,
	1,
};

