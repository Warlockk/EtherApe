/* Automatically generated by po2tbl.sed from etherape.pot.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "libgettext.h"

const struct _msg_ent _msg_tbl[] = {
  {"", 1},
  {"mode of operation", 2},
  {"<ethernet|fddi|ip|tcp>", 3},
  {"set interface to listen to", 4},
  {"<interface name>", 5},
  {"set capture filter", 6},
  {"<capture filter>", 7},
  {"set input file", 8},
  {"<file name>", 9},
  {"don't convert addresses to names", 10},
  {"don't display any node text identification", 11},
  {"do not fade old links", 12},
  {"don't move nodes around", 13},
  {"limits nodes displayed", 14},
  {"<number of nodes>", 15},
  {"Don't show warnings", 16},
  {"set the node color", 17},
  {"color", 18},
  {"set the link color", 19},
  {"set the text color", 20},
  {"Unrecognized mode. Do etherape --help for a list of modes", 21},
  {"We could not load the interface! (%s)", 22},
  {"\
Stack Level is not set to Topmost Recognized Protocol. Please check in the \
preferences dialog that this is what you really want", 23},
  {"Preferences saved", 24},
  {"Error getting device: %s", 25},
  {"Error opening %s : %s - perhaps you need to be root?", 26},
  {"Live device %s opened for capture. pcap_fd: %d", 27},
  {"Can't open both %s and device %s. Please choose one.", 28},
  {"Error opening %s : %s", 29},
  {"%s opened for offline capture", 30},
  {"Link type is Ethernet", 31},
  {"Link type is RAW", 32},
  {"Link type is FDDI", 33},
  {"Link type is Token Ring", 34},
  {"Link type is NULL", 35},
  {"Link type not yet supported", 36},
  {"Mode not available in this device", 37},
  {"Ape mode not yet supported", 38},
  {"Can't use filter:  Couldn't obtain netmask info (%s).", 39},
  {"Unable to parse filter string (%s).", 40},
  {"Can't install filter (%s).", 41},
  {"Status not PAUSE or STOP at start_capture", 42},
  {"Starting live capture", 43},
  {"Starting offline capture", 44},
  {"Status not PLAY at pause_capture", 45},
  {"Pausing live capture", 46},
  {"Pausing offline capture", 47},
  {"Error while trying to pause capture", 48},
  {"Status not PLAY or PAUSE at stop_capture", 49},
  {"Stopping live capture", 50},
  {"Stopping offline capture", 51},
  {"Error while removing capture source in stop_capture", 52},
  {"Capture device stopped or file closed", 53},
  {"Reached default in get_node_id", 54},
  {"Unsopported ape mode in get_link_id", 55},
  {"Creating node: %s. Number of nodes %d", 56},
  {"Creating link: %s-%s. Number of links %d", 57},
  {"Removing node: %s. Number of nodes %d", 58},
  {"Removing link. Number of links %d", 59},
  {"Null packet in check_packet", 60},
  {"No suitables interfaces for capture have been found", 61},
  {"Available interfaces for capture:", 62},
  {"Capture interface set to %s in GUI", 63},
  {"Mode set to %s in GUI", 64},
  {"Status not STOP or PAUSE at gui_start_capture", 65},
  {"Reading data from ", 66},
  {"default interface", 67},
  {" in Token Ring mode", 68},
  {" in FDDI mode", 69},
  {" in Ethernet mode", 70},
  {" in IP mode", 71},
  {" in TCP mode", 72},
  {" in UDP mode", 73},
  {"Diagram started", 74},
  {"Status not PLAY at gui_pause_capture", 75},
  {"Paused", 76},
  {"Diagram paused", 77},
  {"Status not PLAY or PAUSE at gui_stop_capture", 78},
  {"Ready to capture from ", 79},
  {"Diagram stopped", 80},
  {"Number of nodes: ", 81},
  {". Refresh Period: %d", 82},
  {". IDLE.", 83},
  {". TIMEOUT.", 84},
  {"Creating canvas_node: %s. Number of nodes %d", 85},
  {"Main node protocol not found in update_canvas_nodes", 86},
  {"Unknown value or node_size_variable", 87},
  {"Creating canvas_link: %s-%s. Number of links %d", 88},
  {"Main link protocol not found in update_canvas_links", 89},
  {"Link main protocol: %s", 90},
  {"Link main protocol unknown", 91},
  {"Failed to open %s. No TCP or UDP services will be recognized", 92},
  {"Reading TCP and UDP services from %s", 93},
  {"Unable to  parse line %s", 94},
  {"DDP protocols not supported in %s", 95},
  {"EtherApe", 96},
  {"_Capture", 97},
  {"_Mode", 98},
  {"Set Token Ring mode", 99},
  {"Token _Ring", 100},
  {"Set FDDI mode", 101},
  {"_FDDI", 102},
  {"Set Ethernet mode", 103},
  {"_Ethernet", 104},
  {"Set IP mode", 105},
  {"_IP", 106},
  {"Set TCP mode", 107},
  {"_TCP", 108},
  {"Set UDP mode", 109},
  {"_UDP", 110},
  {"_Interfaces", 111},
  {"Start capture", 112},
  {"_Start", 113},
  {"Pause capture", 114},
  {"_Pause", 115},
  {"Stop capture", 116},
  {"St_op", 117},
  {"Show or hide the protocols window", 118},
  {"_Protocols", 119},
  {"Show or hide the toolbar", 120},
  {"_Toolbar", 121},
  {"Show or hide the legend", 122},
  {"_Legend", 123},
  {"Show or hide the status bar", 124},
  {"_Status Bar", 125},
  {"Start", 126},
  {"Pause", 127},
  {"Stop", 128},
  {"Preferences (Ctrl-P)", 129},
  {"Pref.", 130},
  {"Display the protocols window", 131},
  {"Prot.", 132},
  {"Protocols", 133},
  {"Copyright 2001 Juan Toledo", 134},
  {"\
A Graphical Network Browser.\n\
Web: http://etherape.sourceforge.net", 135},
  {"window1", 136},
  {"name", 137},
  {"accumulated", 138},
  {"average", 139},
  {"This message is not here yet. (Don�t tell anybody you saw it ;-) )", 140},
  {"Information", 141},
  {"EtherApe: Preferences", 142},
  {"Save", 143},
  {"Font", 144},
  {"Choose the font used to display text in the diagram", 145},
  {"Protocol Stack Level", 146},
  {"Set what level of the protocol stack is displayed in the legend", 147},
  {"Topmost recognized protocol", 148},
  {"Level 2 (Eg: ETH_II)", 149},
  {"Level 3 (Eg: IP)", 150},
  {"Level 4 (Eg: TCP)", 151},
  {"Level 5 (Eg: HTTP)", 152},
  {"Node size variable", 153},
  {"\
Set the kind of instantenous or accumulated traffic that the node radius \
indicates", 154},
  {"Instant. traffic (In+Out)", 155},
  {"Instant. traffic (Inbound)", 156},
  {"Instant. traffic (Outbound)", 157},
  {"Accum. traffic (In+Out)", 158},
  {"Accum. traffic (Inbound)", 159},
  {"Accum. traffic (Outbound)", 160},
  {"Diagram refresh period (ms)", 161},
  {"Refresh diagram every this many miliseconds", 162},
  {"Diagram Node Timeout (ms)", 163},
  {"\
Remove this node from the diagram after this much time. 0 means never \
timeout.", 164},
  {"Max. Link Width", 165},
  {"Max. Node Radius", 166},
  {"Size Mode", 167},
  {"\
Choose how node radius and link width are calculated as a function of \
average traffic", 168},
  {"Linear", 169},
  {"Logarithmic", 170},
  {"Square Root", 171},
  {"Toggle whether the node names are displayed on top of the nodes", 172},
  {"Hide node names", 173},
  {"Group unknown ports", 174},
  {"Diagram", 175},
  {"Color", 176},
  {"Protocol", 177},
  {"\
Add a new color to the list of colors that can be used to represent protocols", 178},
  {"Add color", 179},
  {"Remove color", 180},
  {"Edit protocol", 181},
  {"Fade link colors to black when there is no traffic", 182},
  {"Fade colors", 183},
  {"Whether or not to use assigned colors for unassigned protocolos", 184},
  {"Cycle assigned colors", 185},
  {"Colors", 186},
  {"Capture filter", 187},
  {"Packet information is averaged for this amount of time", 188},
  {"Averaging Time (ms)", 189},
  {"Delete this link after this much time. 0 means never timeout.", 190},
  {"Link Timeout (ms)", 191},
  {"\
Delete this node from memory after this much time. 0 means never timeout.", 192},
  {"Node Timeout (ms)", 193},
  {"Capture", 194},
  {"Select Font", 195},
  {"EtherApe: Open Capture File", 196},
  {"EtherApe: Select capture file", 197},
  {"EtherApe: Select Capture File", 198},
  {"Select capture file", 199},
  {"Optionally, set a capture filter", 200},
  {"File:", 201},
  {"Filter:", 202},
  {"Name:", 203},
  {"This is a test node name", 204},
  {"Numeric Name:", 205},
  {"Show traffic", 206},
  {"Show protocols", 207},
  {"Instantaneous", 208},
  {"test label", 209},
  {"Accumulated", 210},
  {"Inst. Inbound", 211},
  {"Accu. Outbound", 212},
  {"Accu. Inbound", 213},
  {"Inst. Outbound", 214},
  {"EtherApe: Protocols", 215},
  {"Columns", 216},
  {"Inst Traffic", 217},
  {"Accum Traffic", 218},
  {"Last Heard", 219},
  {"Packets", 220},
  {"Last Heard: ", 221},
  {"label73", 222},
  {"Nodes", 223},
  {"Select color", 224},
  {"ok_button2", 225},
  {"cancel_button2", 226},
  {"help_button1", 227},
  {"EtherApe: assign protocol", 228},
  {"Type protocol name...", 229},
  {"\
Type the name of the protocol that will be assigned to the selected color", 230},
  {"No node_id in on_node_info_delete_event", 231},
  {"No node_info_window in on_node_info_delete_event", 232},
  {"No prot_name in on_prot_info_delete_event", 233},
  {"No prot_info_window in on_prot_info_delete_event", 234},
  {"No info available", 235},
  {"%d\" ago", 236},
  {"%d'%d\" ago", 237},
  {"%d/%d %d:%d", 238},
};

int _msg_tbl_length = 238;
