// font: 8x16 raster "ZDG" font used in various text and gfx routines
//	 (derived from vd2 unit, now standalone part)
// made 26.10.99

// first two bytes are width (in bytes) and height of a char
// each char is 18 bytes;
// first byte is charcode substitution for particular codepage (guess it's cp866);
// second byte is char width (for proportional fonts)
// next 16 bytes are bit data
static uint8_t GUI_font08x16[] = {
	1, 16,
	1,8,0,0,126,129,165,129,129,189,153,129,129,126,0,0,0,0,
	2,8,0,0,126,255,219,255,255,195,231,255,255,126,0,0,0,0,
	3,8,0,0,0,0,108,254,254,254,254,124,56,16,0,0,0,0,
	4,8,0,0,0,0,16,56,124,254,124,56,16,0,0,0,0,0,
	5,8,0,0,0,24,60,60,231,231,231,24,24,60,0,0,0,0,
	6,8,0,0,0,24,60,126,255,255,126,24,24,60,0,0,0,0,
	7,8,0,0,0,0,0,0,24,60,60,24,0,0,0,0,0,0,
	8,8,255,255,255,255,255,255,231,195,195,231,255,255,255,255,255,255,
	9,8,0,0,0,0,0,60,102,66,66,102,60,0,0,0,0,0,
	10,8,255,255,255,255, 255,195,153,189,189,153,195,255,255,255,255,255,
	11,8,0,0,30,14,26, 50,120,204,204,204,204,120,0,0,0,0,
	12,8,0,0,60,102,102,102,102,60, 24,126,24,24,0,0,0,0,
	13,8,0,0,63,51,63,48,48,48,48,112,240,224,0,0,0,0,
	14,8,0,0,127,99,127,99,99,99,99,103,231,230,192,0,0,0,
	15,8,0,0,0,24,24,219,60,231,60,219,24,24,0,0,0,0,
	16,8,0,128,192,224,240,248,252,248,240,224,192,128,0,0,0,0,
	17,8,0,2,6,14,30,62,126,62,30,14,6,2,0,0,0,0,
	18,8,0,0,24,60,126,24,24,24,126,60,24,0,0,0,0,0,
	19,8,0,0,102,102,102,102,102,102,102,0,102,102,0,0,0,0,
	20,8,0,0,127,219,219,219,123,27,27,27,27,27,0,0,0,0,
	21,8,0,124,198,96,56,108,198,198,108,56,12,198,124,0,0,0,
	22,8,0,0,0,0,0,0,0,0,254,254,254,254,0,0,0,0,
	23,8,0,0,24,60,126,24,24,24,126,60,24,126,0,0,0,0,
	24,8,0,0,24,60,126,24,24,24,24,24,24,24,0,0,0,0,
	25,8,0,0,24,24,24,24,24,24,24,126,60,24,0,0,0,0,
	26,8,0,0,0,0,0,24,12,254,12,24,0,0,0,0,0,0,
	27,8,0,0,0,0,0,48,96,254,96,48,0,0,0,0,0,0,
	28,8,0,0,0,0,0,0,192,192,192,254,0,0,0,0,0,0,
	29,8,0,0,0,0,0,40,108,254,108,40,0,0,0,0,0,0,
	30,8,0,0,0,0,16,56,56,124,124,254,254,0,0,0,0,0,
	31,8,0,0,0,0,254,254,124,124,56,56,16,0,0,0,0,0,
	32,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	33,8,0,0,24,24,24,24,24,24,24,0,24,24,0,0,0,0,
	34,8,0,102,102,102,36,0,0,0,0,0,0,0,0,0,0,0,
	35,8,0,0,0,108,108,254,108,108,108,254,108,108,0,0,0,0,
	36,8,24,24,126,192,192,124,6,6,6,6,6,252,24,24,0,0,
	37,8,0,0,0,0,194,198,12,24,48,96,198,134,0,0,0,0,
	38,8,0,0,56,108,108,120,240,216,204,192,192,126,0,0,0,0,
	39,8,0,48,48,48,96,0,0,0,0,0,0,0,0,0,0,0,
	40,8,0,0,12,24,48,48,48,48,48,48,24,12,0,0,0,0,
	41,8,0,0,48,24,12,12,12,12,12,12,24,48,0,0,0,0,
	42,8,0,0,0,0,0,102,60,255,60,102,0,0,0,0,0,0,
	43,8,0,0,0,0,0,24,24,126,24,24,0,0,0,0,0,0,
	44,8,0,0,0,0,0,0,0,0,0,24,24,24,48,0,0,0,
	45,8,0,0,0,0,0,0,0,254,0,0,0,0,0,0,0,0,
	46,8,0,0,0,0,0,0,0,0,0,0,24,24,0,0,0,0,
	47,8,0,0,0,2,6,14,28,56,112,224,192,128,0,0,0,0,
	48,8,0,0,56,108,198,206,222,246,230,198,108,56,0,0,0,0,
	49,8,0,0,24,56,120,24,24,24,24,24,24,24,0,0,0,0,
	50,8,0,0,124,198,6,12,24,48,96,192,192,254,0,0,0,0,
	51,8,0,0,254,6,12,24,48,96,124,6,6,252,0,0,0,0,
	52,8,0,0,204,204,204,254,12,12,12,12,12,12,0,0,0,0,
	53,8,0,0,254,192,192,192,192,192,252,6,6,252,0,0,0,0,
	54,8,0,0,60,96,192,192,192,192,252,198,198,124,0,0,0,0,
	55,8,0,0,254,6,12,24,48,48,48,48,48,48,0,0,0,0,
	56,8,0,0,124,198,198,124,198,198,198,198,198,124,0,0,0,0,
	57,8,0,0,124,198,198,126,6,6,6,6,12,120,0,0,0,0,
	58,8,0,0,0,0,24,24,0,0,0,24,24,0,0,0,0,0,
	59,8,0,0,0,0,24,24,0,0,0,24,24,48,0,0,0,0,
	60,8,0,0,0,6,12,24,48,96,48,24,12,6,0,0,0,0,
	61,8,0,0,0,0,0,126,0,0,126,0,0,0,0,0,0,0,
	62,8,0,0,0,96,48,24,12,6,12,24,48,96,0,0,0,0,
	63,8,0,0,124,198,198,12,24,24,24,0,24,24,0,0,0,0,
	64,8,0,0,0,124,198,198,222,222,222,220,192,124,0,0,0,0,
	65,8,0,0,56,108,198,254,198,198,198,198,198,198,0,0,0,0,
	66,8,0,0,252,198,198,252,198,198,198,198,198,252,0,0,0,0,
	67,8,0,0,62,96,192,192,192,192,192,192,96,62,0,0,0,0,
	68,8,0,0,248,204,198,198,198,198,198,198,204,248,0,0,0,0,
	69,8,0,0,254,192,192,254,192,192,192,192,192,254,0,0,0,0,
	70,8,0,0,254,192,192,254,192,192,192,192,192,192,0,0,0,0,
	71,8,0,0,62,96,192,192,198,198,198,198,102,62,0,0,0,0,
	72,8,0,0,198,198,198,254,198,198,198,198,198,198,0,0,0,0,
	73,8,0,0,24,24,24,24,24,24,24,24,24,24,0,0,0,0,
	74,8,0,0,6,6,6,6,6,6,6,6,12,248,0,0,0,0,
	75,8,0,0,198,198,204,248,204,198,198,198,198,198,0,0,0,0,
	76,8,0,0,192,192,192,192,192,192,192,192,192,254,0,0,0,0,
	77,8,0,0,130,198,238,254,214,198,198,198,198,198,0,0,0,0,
	78,8,0,0,198,230,246,222,206,198,198,198,198,198,0,0,0,0,
	79,8,0,0,124,198,198,198,198,198,198,198,198,124,0,0,0,0,
	80,8,0,0,252,198,198,252,192,192,192,192,192,192,0,0,0,0,
	81,8,0,0,124,198,198,198,198,198,198,198,222,124,24,254,0,0,
	82,8,0,0,252,198,198,252,216,204,198,198,198,198,0,0,0,0,
	83,8,0,0,126,192,192,124,6,6,6,6,6,252,0,0,0,0,
	84,8,0,0,126,24,24,24,24,24,24,24,24,24,0,0,0,0,
	85,8,0,0,198,198,198,198,198,198,198,198,198,124,0,0,0,0,
	86,8,0,0,198,198,198,198,198,198,198,108,56,16,0,0,0,0,
	87,8,0,0,198,198,198,198,198,214,254,238,198,130,0,0,0,0,
	88,8,0,0,198,198,108,56,108,198,198,198,198,198,0,0,0,0,
	89,8,0,0,102,102,102,60,24,24,24,24,24,24,0,0,0,0,
	90,8,0,0,254,6,6,12,24,48,96,192,192,254,0,0,0,0,
	91,8,0,0,60,48,48,48,48,48,48,48,48,60,0,0,0,0,
	92,8,0,0,0,128,192,224,112,56,28,14,6,2,0,0,0,0,
	93,8,0,0,60,12,12,12,12,12,12,12,12,60,0,0,0,0,
	94,8,16,56,108,198,0,0,0,0,0,0,0,0,0,0,0,0,
	95,8,0,0,0,0,0,0,0,0,0,0,0,0,0,255,0,0,
	96,8,48,48,24,0,0,0,0,0,0,0,0,0,0,0,0,0,
	97,8,0,0,0,0,0,124,6,126,198,198,198,126,0,0,0,0,
	98,8,0,0,192,192,192,252,198,198,198,198,198,252,0,0,0,0,
	99,8,0,0,0,0,0,126,192,192,192,192,192,126,0,0,0,0,
	100,8,0,0,6,6,6,126,198,198,198,198,198,126,0,0,0,0,
	101,8,0,0,0,0,0,124,198,254,192,192,192,126,0,0,0,0,
	102,8,0,0,60,96,96,252,96,96,96,96,96,96,0,0,0,0,
	103,8,0,0,0,0,0,126,198,198,198,198,198,126,6,6,252,0,
	104,8,0,0,192,192,192,252,198,198,198,198,198,198,0,0,0,0,
	105,8,0,0,24,24,0,24,24,24,24,24,24,24,0,0,0,0,
	106,8,0,0,6,6,0,6,6,6,6,6,6,6,6,6,124,0,
	107,8,0,0,192,192,192,198,204,248,204,198,198,198,0,0,0,0,
	108,8,0,0,56,24,24,24,24,24,24,24,24,24,0,0,0,0,
	109,8,0,0,0,0,0,252,214,214,214,214,214,214,0,0,0,0,
	110,8,0,0,0,0,0,252,198,198,198,198,198,198,0,0,0,0,
	111,8,0,0,0,0,0,124,198,198,198,198,198,124,0,0,0,0,
	112,8,0,0,0,0,0,252,198,198,198,198,198,252,192,192,192,0,
	113,8,0,0,0,0,0,126,198,198,198,198,198,126,6,6,6,0,
	114,8,0,0,0,0,0,222,240,224,192,192,192,192,0,0,0,0,
	115,8,0,0,0,0,0,126,192,192,124,6,6,252,0,0,0,0,
	116,8,0,0,48,48,48,254,48,48,48,48,48,30,0,0,0,0,
	117,8,0,0,0,0,0,198,198,198,198,198,198,126,0,0,0,0,
	118,8,0,0,0,0,0,102,102,102,102,102,60,24,0,0,0,0,
	119,8,0,0,0,0,0,198,198,214,254,238,198,130,0,0,0,0,
	120,8,0,0,0,0,0,198,198,108,56,108,198,198,0,0,0,0,
	121,8,0,0,0,0,0,198,198,198,198,198,198,126,6,6,252,0,
	122,8,0,0,0,0,0,254,12,24,48,96,192,254,0,0,0,0,
	123,8,0,0,14,24,24,24,112,24,24,24,24,14,0,0,0,0,
	124,8,0,0,24,24,24,24,0,24,24,24,24,24,0,0,0,0,
	125,8,0,0,112,24,24,24,14,24,24,24,24,112,0,0,0,0,
	126,8,0,0,118,220,0,0,0,0,0,0,0,0,0,0,0,0,
	127,8,0,0,0,0,16,56,108,198,198,198,254,0,0,0,0,0,

	192,8,24,24,24,24,24,24,24,31,0,0,0,0,0,0,0,0,		//128
	193,8,24,24,24,24,24,24,24,255,0,0,0,0,0,0,0,0,		//129
	194,8,0,0,0,0,0,0,0,255,24,24,24,24,24,24,24,24,	//130
	195,8,24,24,24,24,24,24,24,31,24,24,24,24,24,24,24,24,	//131
	196,8,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0,0,				//132
	197,8,24,24,24,24,24,24,24,255,24,24,24,24,24,24,24,24,	//133
	198,8,24,24,24,24,24,31,24,31,24,24,24,24,24,24,24,24,	//134
	199,8,54,54,54,54,54,54,54,55,54,54,54,54,54,54,54,54,	//135
	200,8,54,54,54,54,54,55,48,63,0,0,0,0,0,0,0,0,		//136
	201,8,0,0,0,0,0,63,48,55,54,54,54,54,54,54,54,54,	//137
	202,8,54,54,54,54,54,247,0,255,0,0,0,0,0,0,0,0,		//138
	203,8,0,0,0,0,0,255,0,247,54,54,54,54,54,54,54,54,	//139
	204,8,54,54,54,54,54,55,48,55,54,54,54,54,54,54,54,54,	//140
	205,8,0,0,0,0,0,255,0,255,0,0,0,0,0,0,0,0,		//141
	206,8,54,54,54,54,54,247,0,247,54,54,54,54,54,54,54,54,	//142
	207,8,24,24,24,24,24,255,0,255,0,0,0,0,0,0,0,0,		//143
	208,8,54,54,54,54,54,54,54,255,0,0,0,0,0,0,0,0,		//144
	209,8,0,0,0,0,0,255,0,255,24,24,24,24,24,24,24,24,	//145
	210,8,0,0,0,0,0,0,0,255,54,54,54,54,54,54,54,54,	//146
	211,8,54,54,54,54,54,54,54,63,0,0,0,0,0,0,0,0,		//147
	212,8,24,24,24,24,24,31,24,31,0,0,0,0,0,0,0,0,		//148
	213,8,0,0,0,0,0,31,24,31,24,24,24,24,24,24,24,24,	//149
	214,8,0,0,0,0,0,0,0,63,54,54,54,54,54,54,54,54,		//150
	215,8,54,54,54,54,54,54,54,255,54,54,54,54,54,54,54,54,	//151
	216,8,24,24,24,24,24,255,24,255,24,24,24,24,24,24,24,24,//152
	217,8,24,24,24,24,24,24,24,248,0,0,0,0,0,0,0,0,		//153
	218,8,0,0,0,0,0,0,0,31,24,24,24,24,24,24,24,24,		//154
	219,8,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,//155
	220,8,0,0,0,0,0,0,0,255,255,255,255,255,255,255,255,255,//156
	221,8,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,//157
	222,8,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,	//158
	223,8,255,255,255,255,255,255,255,0,0,0,0,0,0,0,0,0,	//159

	176,8,17,68,17,68,17,68,17,68,17,68,17,68,17,68,17,68,	//160	
	177,8,85,170,85,170,85,170,85,170,85,170,85,170,85,170,85,170,
	178,8,221,119,221,119,221,119,221,119,221,119,221,119,221,119,221,119,
	179,8,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,
	180,8,24,24,24,24,24,24,24,248,24,24,24,24,24,24,24,24,
	181,8,24,24,24,24,24,248,24,248,24,24,24,24,24,24,24,24,
	182,8,54,54,54,54,54,54,54,246,54,54,54,54,54,54,54,54,
	183,8,0,0,0,0,0,0,0,254,54,54,54,54,54,54,54,54,
	184,8,0,0,0,0,0,248,24,248,24,24,24,24,24,24,24,24,
	185,8,54,54,54,54,54,246,6,246,54,54,54,54,54,54,54,54,
	186,8,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,
	187,8,0,0,0,0,0,254,6,246,54,54,54,54,54,54,54,54,
	188,8,54,54,54,54,54,246,6,254,0,0,0,0,0,0,0,0,
	189,8,54,54,54,54,54,54,54,254,0,0,0,0,0,0,0,0,
	190,8,24,24,24,24,24,248,24,248,0,0,0,0,0,0,0,0,
	191,8,0,0,0,0,0,0,0,248,24,24,24,24,24,24,24,24,

	240,8,0,0,0,0,254,0,0,254,0,0,254,0,0,0,0,0,
	241,8,0,0,0,0,24,24,126,24,24,0,0,255,0,0,0,0,
	242,8,0,0,0,48,24,12,6,12,24,48,0,126,0,0,0,0,
	243,8,0,0,0,12,24,48,96,48,24,12,0,126,0,0,0,0,
	244,8,0,0,14,27,27,24,24,24,24,24,24,24,24,24,24,24,
	245,8,24,24,24,24,24,24,24,24,216,216,216,112,0,0,0,0,
	246,8,0,0,0,0,24,24,0,126,0,24,24,0,0,0,0,0,
	247,8,0,0,0,0,0,118,220,0,118,220,0,0,0,0,0,0,
	248,8,0,56,108,108,56,0,0,0,0,0,0,0,0,0,0,0,
	249,8,0,0,0,0,0,0,0,24,24,0,0,0,0,0,0,0,
	250,8,0,0,0,0,0,0,0,0,24,0,0,0,0,0,0,0,
	251,8,0,0,1,3,2,6,4,76,104,56,16,0,0,0,0,0,
	252,8,0,0,0,60,66,153,165,161,165,153,66,60,0,0,0,0,
	253,8,0,112,152,48,96,192,248,0,0,0,0,0,0,0,0,0,
	254,8,0,0,0,0,124,124,124,124,124,124,124,0,0,0,0,0,
	255,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

	128,8,0,0,56,108,198,254,198,198,198,198,198,198,0,0,0,0,	//192
	129,8,0,0,254,192,192,192,192,192,252,198,198,252,0,0,0,0,
	130,8,0,0,252,198,198,252,198,198,198,198,198,252,0,0,0,0,
	131,8,0,0,254,192,192,192,192,192,192,192,192,192,0,0,0,0,
	132,8,0,0,30,54,102,102,102,102,102,102,102,255,195,195,0,0,
	133,8,0,0,254,192,192,254,192,192,192,192,192,254,0,0,0,0,
	134,8,0,0,214,214,214,254,124,214,214,214,214,214,0,0,0,0,
	135,8,0,0,252,6,6,60,6,6,6,6,6,252,0,0,0,0,
	136,8,0,0,198,198,206,222,246,230,198,198,198,198,0,0,0,0,
	137,8,56,56,198,198,206,222,246,230,198,198,198,198,0,0,0,0,
	138,8,0,0,198,198,204,248,204,198,198,198,198,198,0,0,0,0,
	139,8,0,0,30,54,102,198,198,198,198,198,198,198,0,0,0,0,
	140,8,0,0,198,238,254,254,214,198,198,198,198,198,0,0,0,0,
	141,8,0,0,198,198,198,254,198,198,198,198,198,198,0,0,0,0,
	142,8,0,0,124,198,198,198,198,198,198,198,198,124,0,0,0,0,
	143,8,0,0,254,198,198,198,198,198,198,198,198,198,0,0,0,0,
	144,8,0,0,252,198,198,252,192,192,192,192,192,192,0,0,0,0,
	145,8,0,0,62,96,192,192,192,192,192,192,96,62,0,0,0,0,
	146,8,0,0,126,24,24,24,24,24,24,24,24,24,0,0,0,0,
	147,8,0,0,198,198,198,126,6,6,6,6,6,252,0,0,0,0,
	148,8,0,24,126,219,219,219,219,219,219,126,24,24,0,0,0,0,
	149,8,0,0,198,198,108,56,108,198,198,198,198,198,0,0,0,0,
	150,8,0,0,204,204,204,204,204,204,204,204,204,254,6,6,0,0,
	151,8,0,0,198,198,198,126,6,6,6,6,6,6,0,0,0,0,
	152,8,0,0,219,219,219,219,219,219,219,219,219,255,0,0,0,0,
	153,8,0,0,219,219,219,219,219,219,219,219,219,255,3,3,0,0,
	154,8,0,0,240,48,48,48,48,48,60,54,54,60,0,0,0,0,
	155,8,0,0,195,195,195,195,195,195,243,219,219,243,0,0,0,0,
	156,8,0,0,192,192,192,192,192,192,252,198,198,252,0,0,0,0,
	157,8,0,0,252,6,6,254,6,6,6,6,6,252,0,0,0,0,
	158,8,0,0,206,219,219,219,251,219,219,219,219,206,0,0,0,0,
	159,8,0,0,126,198,198,126,30,54,102,198,198,198,0,0,0,0,	//223
	160,8,0,0,0,0,0,124,6,126,198,198,198,126,0,0,0,0,			//�-224
	161,8,0,0,126,192,192,252,198,198,198,198,198,124,0,0,0,0,	//�
	162,8,0,0,0,0,0,252,198,252,198,198,198,252,0,0,0,0,		//�
	163,8,0,0,0,0,0,254,192,192,192,192,192,192,0,0,0,0,		//�
	164,8,0,0,0,0,0,30,54,102,102,102,102,255,195,195,0,0,		//�
	165,8,0,0,0,0,0,124,198,254,192,192,192,126,0,0,0,0,		//�
	166,8,0,0,0,0,0,214,214,124,214,214,214,214,0,0,0,0,		//�
	167,8,0,0,0,0,0,252,6,6,28,6,6,252,0,0,0,0,					//�
	168,8,0,0,0,0,0,198,206,222,246,230,198,198,0,0,0,0,		//�
	169,8,0,0,0,56,56,198,206,222,246,230,198,198,0,0,0,0,		//�
	170,8,0,0,0,0,0,198,204,248,204,198,198,198,0,0,0,0,		//�
	171,8,0,0,0,0,0,62,102,198,198,198,198,198,0,0,0,0,			//�
	172,8,0,0,0,0,0,198,238,254,214,198,198,198,0,0,0,0,		//�
	173,8,0,0,0,0,0,198,198,254,198,198,198,198,0,0,0,0,		//�
	174,8,0,0,0,0,0,124,198,198,198,198,198,124,0,0,0,0,		//�
	175,8,0,0,0,0,0,254,198,198,198,198,198,198,0,0,0,0,		//�-239

	224,8,0,0,0,0,0,252,198,198,198,198,198,252,192,192,192,0,	//240
	225,8,0,0,0,0,0,126,192,192,192,192,192,126,0,0,0,0,
	226,8,0,0,0,0,0,126,24,24,24,24,24,24,0,0,0,0,
	227,8,0,0,0,0,0,198,198,198,198,198,198,126,6,6,252,0,
	228,8,0,0,0,0,24,126,219,219,219,219,219,126,24,24,24,0,
	229,8,0,0,0,0,0,198,198,108,56,108,198,198,0,0,0,0,
	230,8,0,0,0,0,0,204,204,204,204,204,204,254,6,6,0,0,
	231,8,0,0,0,0,0,198,198,198,126,6,6,6,0,0,0,0,
	232,8,0,0,0,0,0,214,214,214,214,214,214,254,0,0,0,0,
	233,8,0,0,0,0,0,214,214,214,214,214,214,254,3,3,0,0,
	234,8,0,0,0,0,0,240,48,48,62,51,51,62,0,0,0,0,
	235,8,0,0,0,0,0,198,198,198,246,218,218,242,0,0,0,0,
	236,8,0,0,0,0,0,96,96,96,124,102,102,124,0,0,0,0,
	237,8,0,0,0,0,0,252,6,6,254,6,6,252,0,0,0,0,
	238,8,0,0,0,0,0,206,219,219,251,219,219,206,0,0,0,0,
	239,8,0,0,0,0,0,126,198,126,54,102,198,198,0,0,0,0
};