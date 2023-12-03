#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#include "encryption.h"


/******************************************************************************/
/*** Macros                                                                 ***/
/******************************************************************************/
#define ARRAY_SIZE(arr)         (sizeof(arr) / sizeof((arr)[0]))

/* Option flags */
#define FLAGS__INVALID		(0 << 0)
#define FLAGS__VALID		(1 << 0)

#undef DEBUG


/******************************************************************************/
/*** Types                                                                  ***/
/******************************************************************************/
struct option_t {
	unsigned int           seed_ndx;       /* Option seed index */
	unsigned int           flags;          /* Option flags */
	char                   *description;   /* Option description string */
};

struct instrument_t {
	char                   *name;          /* Instrument name string */
	unsigned int           nr_of_options;  /* Nr. of options is the list below */
	const struct option_t  *options;       /* Instrument option list */
};


/******************************************************************************/
/*** Global data                                                            ***/
/******************************************************************************/
/* FSP option list */
static const struct option_t  fsp_options[] = {
	{  0, FLAGS__VALID,   "K5  - GSM/EDGE Application Firmware" },
	{  1, FLAGS__VALID,   "K7  - AM/FM/PM Measurement Demodulator" },
	{  2, FLAGS__VALID,   "B17 - IQ Online" },
	{  3, FLAGS__INVALID, "None" },
	{  4, FLAGS__VALID,   "K84 - 1xEV-DO BTS Application Firmware" },
	{  5, FLAGS__VALID,   "K84 - 1xEV-DO MS  Application Firmware" },
	{  6, FLAGS__INVALID, "None" },
	{  7, FLAGS__INVALID, "FSP-B15 FSP-B70 FS-K7" },
	{  8, FLAGS__INVALID, "K72 prior K74" },
	{  9, FLAGS__INVALID, "None" },
	{ 10, FLAGS__VALID,   "K9  - Power Meter" },
	{ 11, FLAGS__INVALID, "FSP-B15 FS-K7" },
	{ 12, FLAGS__INVALID, "None" },
	{ 13, FLAGS__INVALID, "None" },
	{ 14, FLAGS__VALID,   "K76 - 3GPP TD-SCDMA BTS Application Firmware" },
	{ 15, FLAGS__VALID,   "K77 - 3GPP TD-SCDMA MS  Application Firmware" },
	{ 16, FLAGS__VALID,   "K30 - Noise Figure Measurament" },
	{ 17, FLAGS__VALID,   "K82 - CDMA2000 BTS Application Firmware" },
	{ 18, FLAGS__VALID,   "K83 - CDMA2000 MS  Application Firmware" },
	{ 19, FLAGS__VALID,   "K8  - Bluetooth Application Firmware" },
	{ 20, FLAGS__VALID,   "K40 - Phase Noise Measurament" },
	{ 21, FLAGS__INVALID, "None" },
	{ 22, FLAGS__INVALID, "None" },
	{ 23, FLAGS__INVALID, "None" },
	{ 24, FLAGS__INVALID, "None" },
	{ 25, FLAGS__INVALID, "31 days trial period" },
	{ 26, FLAGS__INVALID, "78 days trial period" },
	{ 27, FLAGS__INVALID, "None" },
	{ 28, FLAGS__INVALID, "None" },
	{ 29, FLAGS__INVALID, "Frequency Extension" },
	{ 30, FLAGS__INVALID, "None" },
	{ 31, FLAGS__INVALID, "Trasducer Set" },
	{ 32, FLAGS__INVALID, "None" },
	{ 33, FLAGS__INVALID, "None" },
	{ 34, FLAGS__INVALID, "None" },
	{ 35, FLAGS__INVALID, "None" },
	{ 36, FLAGS__INVALID, "None" },
	{ 37, FLAGS__INVALID, "None" },
	{ 38, FLAGS__INVALID, "None" },
	{ 39, FLAGS__INVALID, "None" },
	{ 40, FLAGS__INVALID, "None" },
	{ 41, FLAGS__INVALID, "None" },
	{ 42, FLAGS__INVALID, "None" },
	{ 43, FLAGS__INVALID, "None" },
	{ 44, FLAGS__INVALID, "None" },
	{ 45, FLAGS__INVALID, "None" },
	{ 46, FLAGS__INVALID, "None" },
	{ 47, FLAGS__INVALID, "None" },
	{ 48, FLAGS__INVALID, "None" },
	{ 49, FLAGS__INVALID, "None" }
};

/* FSIQ3 option list */
static const struct option_t  fsiq3_options[] = {
	{  0, FLAGS__VALID, "K10 - GSM MS Analyzer" },
	{  1, FLAGS__VALID, "K11 - GSM BTS Analyzer" },
	{  2, FLAGS__VALID, "B5  - FFT" },
	{  3, FLAGS__VALID, "ESI (Does not show on FSIQ3)" },
	{  4, FLAGS__VALID, "Kxx - Receiver Mode" },
	{  5, FLAGS__VALID, "FSIQ" },
	{  6, FLAGS__VALID, "K71 - CDMA ONE BTS Analyzer" },
	{  7, FLAGS__VALID, "K72 - W-CDMA BTS Analyzer" },
	{  8, FLAGS__VALID, "K12 - Digital Standard ICO" },
	{  9, FLAGS__VALID, "K20 - EDGE MS Analyzer Extension" },
	{ 10, FLAGS__VALID, "K21 - EDGE BTS Analyzer Extension" },
	{ 11, FLAGS__VALID, "K73 - W-CDMA MS Analyzer" },
	{ 12, FLAGS__VALID, "K30 - 850 MHz Extension for K10/K20" },
	{ 13, FLAGS__VALID, "K31 - 850 MHz Extension for K11/K21" },
	{ 14, FLAGS__VALID, "AFB (Does not show on FSIQ3)" },
	{ 15, FLAGS__VALID, "Freq limit 41GHz (Does not show on FSIQ3)" }
};

/* FSQ8 option list */
static const struct option_t fsq8_options[] = {
	{0, FLAGS__VALID, "K5  - GSM/EDGE Application Firmware"},
	{1, FLAGS__VALID, "K7  - AM/FM/PM Measurement Demodulator"},
	{2, FLAGS__VALID, "None"},
	{3, FLAGS__VALID, "None"},
	{4, FLAGS__VALID, "K84 - 1xEV-DO BTS Application Firmware"},
	{5, FLAGS__VALID, "K85 - 1xEV-DO MS  Application Firmware"},
	{6, FLAGS__VALID, "K90 - FSQ WLAN 802.11 a"},
	{7, FLAGS__VALID, "K72 - WCDMA BTS Analyser"},
	{8, FLAGS__VALID, "K74 - WCDMA BTS Analyser"},
	{9, FLAGS__VALID, "None"},
	{10, FLAGS__VALID, "K9 - Power Meter"},
	{11, FLAGS__VALID, "K73 - WCDMA MS Analyser"},
	{12, FLAGS__VALID, "None"},
	{13, FLAGS__VALID, "K90UP - FSQ WLAN 802.11a,b,g"},
	{14, FLAGS__VALID, "K76 - 3GPP TD-SCDMA BTS Analyser"},
	{15, FLAGS__VALID, "K77 - 3GPP TD-SCDMA MS  Analyser"},
	{16, FLAGS__VALID, "K30 - Noise Figure Measurament"},
	{17, FLAGS__VALID, "K82 - CDMA2000 BTS Analyser"},
	{18, FLAGS__VALID, "K83 - CDMA2000 MS  Analyser"},
	{19, FLAGS__VALID, "K8  - Bluetooth"},
	{20, FLAGS__VALID, "K40 - Phase Noise Measurament"},
	{21, FLAGS__VALID, "K70 - VSA"},
	{22, FLAGS__VALID, "None"},
	{23, FLAGS__VALID, "None"},
	{24, FLAGS__VALID, "None"},
	{25, FLAGS__VALID, "31 days trial period"},
	{26, FLAGS__VALID, "141 days trial period"},
	{27, FLAGS__VALID, "K91 - FSQ WLAN 802.11a,b,g "},
	{28, FLAGS__VALID, "None"},
	{29, FLAGS__VALID, "B8 - Frequency Extension"},
	{30, FLAGS__VALID, "K92 - FSQ802.16"},
	{31, FLAGS__VALID, "Trasducer Set"},
	{32, FLAGS__VALID, "None"},
	{33, FLAGS__VALID, "K901 - 277 days trail period"},
	{34, FLAGS__VALID, "K902 - 414 days trail period"},
	{35, FLAGS__VALID, "K92UP - FSQ802.16 E"},
	{36, FLAGS__VALID, "K93 - FSQ802.16 E"},
	{37, FLAGS__INVALID, "None"},
	{38, FLAGS__INVALID, "None"},
	{39, FLAGS__VALID, "K100 - LTE FDD Downlink"},
	{40, FLAGS__VALID, "K101 - LTE FDD Uplink"},
	{41, FLAGS__VALID, "None"},
	{42, FLAGS__VALID, "K15 - VOR/ILS Avionics Demodulator"},
	{43, FLAGS__INVALID, "None"},
	{44, FLAGS__VALID, "K94 - FSQ802.16 E MIMO"},
	{45, FLAGS__VALID, "K102 - LTE Downlink MIMO"},
	{46, FLAGS__VALID, "K103 - LTE Uplink MIMO"},
	{47, FLAGS__VALID, "K96 - OFDM VSA"},
	{48, FLAGS__VALID, "K74UP - WCDMA BTS Analyser"},
	{49, FLAGS__VALID, "None"},
	{50, FLAGS__VALID, "K400 RFEX-F Launcher"},
	{51, FLAGS__VALID, "None"},
	{52, FLAGS__VALID, "K110 - TETRA 2"},
	{53, FLAGS__VALID, "K10 - GSM EDGE (FW extension packet required)"},
	{54, FLAGS__INVALID, "None"},
	{55, FLAGS__VALID, "K104 - LTE TDD Downlink"},
	{56, FLAGS__VALID, "K105 - LTE TDD Uplink"},
	{57, FLAGS__VALID, "K106 - LTE TDD/FDD Upgrade"},
	{58, FLAGS__VALID, "K5UP - GSM/EDGE Application Firmware"},
	{59, FLAGS__VALID, "K150 - Support for User Calibration"},
	{60, FLAGS__VALID, "None"},
	{61, FLAGS__VALID, "None"},
	{62, FLAGS__VALID, "None"},
	{63, FLAGS__VALID, "None"},
	{64, FLAGS__VALID, "None"},
	{65, FLAGS__VALID, "None"},
	{66, FLAGS__VALID, "None"},
	{67, FLAGS__VALID, "None"},
	{68, FLAGS__VALID, "Probably Invalid: Throws Exception"},
	{69, FLAGS__VALID, "None"},
};

/* Instrument list */
static const struct instrument_t  instruments[] = {
	{"FSP",    ARRAY_SIZE(fsp_options),    fsp_options},
//	{"FSEA20", ARRAY_SIZE(fsea20_options), fsea20_options},
//	{"FSEA30", ARRAY_SIZE(fsea30_options), fsea30_options},
//	{"FSEB20", ARRAY_SIZE(fseb20_options), fseb20_options},
//	{"FSEB30", ARRAY_SIZE(fseb30_options), fseb30_options},
//	{"FSEM20", ARRAY_SIZE(fsem20_options), fsem20_options},
//	{"FSEM21", ARRAY_SIZE(fsem21_options), fsem21_options},
//	{"FSEM30", ARRAY_SIZE(fsem30_options), fsem30_options},
//	{"FSEM31", ARRAY_SIZE(fsem31_options), fsem31_options},
	{"FSIQ3",  ARRAY_SIZE(fsiq3_options),  fsiq3_options},
//	{"FSIQ7",  ARRAY_SIZE(fsiq7_options),  fsiq7_options},
//	{"FSIQ26", ARRAY_SIZE(fsiq26_options), fsiq26_options},
//	{"FSIQ40", ARRAY_SIZE(fsiq40_options), fsiq40_options},
    {"FSQ8",   ARRAY_SIZE(fsq8_options),   fsq8_options}
};


/******************************************************************************/
/*** Static functions                                                       ***/
/******************************************************************************/
static int get_serialnr(const char *optarg, unsigned long *serialnr)
{
	char  *endptr;

	/* Retrieve the number before the slash */
	*serialnr = 1000 * strtoul(optarg, &endptr, 10);

	/* Bail with an error if no slash follows */
	if (*endptr != '/')
		goto err_serialnr;

	/* Bail with an error if nothing follows the slash */
	endptr++;
	if (*endptr == '\0')
		goto err_serialnr;

	/* Add the number following the slash */
	*serialnr += strtoul(endptr, &endptr, 10);

	/* Bail with an error if anything follows */
	if (*endptr != '\0')
		goto err_serialnr;

	return 0;

err_serialnr:
	fprintf(stderr, "Invalid serial number '%s'\n", optarg);
	fprintf(stderr, "Serial numbers are formatted like 123456/789\n");

	return -1;
}

void print_supported_instruments(){
	fprintf(stderr, "Supported instruments are:\n");
	for (int ndx = 0; ndx < ARRAY_SIZE(instruments); ndx++)
		fprintf(stderr, "%s\n", instruments[ndx].name);
}

static int get_instrument(const char *optarg, const struct instrument_t **instrument)
{
	int  ndx;

	for (ndx = 0; ndx < ARRAY_SIZE(instruments); ndx++)
		if (!strcasecmp(optarg, instruments[ndx].name)) {
			*instrument = &instruments[ndx];
			return 0;
		}

	fprintf(stderr, "Unsupported instrument '%s'\n", optarg);
	print_supported_instruments();
	fprintf(stderr, "Omit the instrument option for raw keys\n");
	return -1;
}


static void print_options(const struct instrument_t *instrument, unsigned long serialnr)
{
	unsigned int  ndx;

	fprintf(stdout, "Option keys for %s with serial number %lu/%.3lu are:\n", instrument->name, serialnr / 1000, serialnr % 1000);

	/* Iterate through all options */
	for (ndx = 0; ndx < instrument->nr_of_options; ndx++) {
		/* Check if option is valid */
		if (!(instrument->options[ndx].flags & FLAGS__VALID))
			continue;

		fprintf(stdout, "%010u - %s\n", fsxx_encrypt(seeds[instrument->options[ndx].seed_ndx], serialnr), instrument->options[ndx].description);
	}
}


static void print_raw_keys(unsigned long serialnr)
{
	unsigned int  ndx;

	fprintf(stdout, "Raw keys for instrument with serial number %lu/%.3lu are:\n", serialnr / 1000, serialnr % 1000);

	/* Iterate through all seeds */
	for (ndx = 0; ndx < nr_of_seeds; ndx++)
		fprintf(stdout, "%.3u: %010u\n", ndx, fsxx_encrypt(seeds[ndx], serialnr));
}


static void print_usage(char const * const basename)
{
	fprintf(stderr, "Usage: %s -s serialnr [-t type]\n", basename);
	fprintf(stderr, "Example: %s -s 120033/008 -t FSQ8\n\n", basename);
	fprintf(stderr, "The device type is optional. If your device is not supported (yet), \nthere is still a good chance that the keygen will work.\n");
	print_supported_instruments();
}

/*****************************************************************************/
/*** Functions                                                             ***/
/*****************************************************************************/
int main(int argc, char* argv[])
{
	int                  result = EXIT_SUCCESS;
	int                  arg;
	unsigned long        serialnr = 0;
	const struct instrument_t  *instrument = NULL;

	if(argc <= 1){
		print_usage(basename(argv[0]));
		return EXIT_FAILURE;
	}

	/* Process the command line arguments */
	while ((arg = getopt(argc, argv, "hs:t:")) != -1) {
		switch (arg) {
		case 's':
			if (get_serialnr(optarg, &serialnr))
				return EXIT_FAILURE;
			break;

		case 't':
			if (get_instrument(optarg, &instrument))
				return EXIT_FAILURE;
			break;

		default:
			result = EXIT_FAILURE;
		case 'h':
			print_usage(basename(argv[0]));
			return result;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc) {
		fprintf(stderr, "Stdin not supported\n");
		result = EXIT_FAILURE;
		goto err_arg;
	}

#ifdef DEBUG
	fprintf(stderr, "decrypt(\"0123456789\", %ld) = 0x%.8x\n", serialnr, fsxx_decrypt("0123456789", serialnr));
	fprintf(stderr, "encrypt(0x%.8x,   %ld) = 0x%.8x\n", seeds[0], serialnr, fsxx_encrypt(seeds[0], serialnr));
#endif /* DEBUG */

	/* No output if no serial number is given */
	if (!serialnr)
		goto err_none;

	if (instrument)
		print_options(instrument, serialnr);
	else
		print_raw_keys(serialnr);

err_none:
err_arg:
	return result;
}
