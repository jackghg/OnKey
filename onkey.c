// OnKey 0.5
// gcc onkey.c -o onkey -lX11 -lXtst `pkg-config --cflags gtk+-2.0 pkg-config --libs gtk+-2.0`
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <gdk/gdk.h>

Window w;
GtkWidget *win, *table, *bt, *bts;
GdkColor sac, sfc;
Display *dpy;
KeySym sym = 0;
KeyCode shic, code = 0;
char ly[15], tc[5];
int co, ln;
short pg = 1, sbt = 0;
bool nkc = 0, shi = 0, ws = 0, ei = 0, fl = 0;

// --- Variables to customize ---
// number of columns, number of rows per page, number of pages
const short nc = 10, nr = 5, tp = 2;
// the main keys table
char keya[10][10][8] = {{"q","w","e","r","t","y","u","i","o","p"},
                       {"a","s","d","f","g","h","j","k","l","^^^e"},
                       {"z","x","c","v","b","n","m","^^n,","^^n.","^^^S"},
                       {"^^^p","^^^x","^^s?","^^s!","^^n'","^^s\"","^^s:","^^n=","^^n/","^^^s"},
                       {"1","2","3","4","5","6","7","8","9","0"},
                       {"^^n;","^^s(","^^s)","^^n-","^^s+","^^s*","^^s%","^^s_","^^s|","^^s@"},
                       {"^^s&","^^s#","^^n<","^^s>","^^n\\","^^n`","U00B0","^^s^","^^^u","^^^e"},
                       {"^^n[","^^n]","^^s{","^^s}","^^s~","^^s$","U20AC","^^^l","^^^d","^^^r"},
                       {"^^^P","^^^p","^^^t","U00AB","U00BB","U00F7","U00D7","U00B1","U00A7","U00E7"},
                       {"U00A2","U00A3","U00A5","U00F8","U00BD","U00B5","U03A3","U03A9","U0394","U1D11E"}};
// the up-case keys table
char ukey[5][10][8] = {{"^^SQ","^^SW","^^SE","^^SR","^^ST","^^SY","^^SU","^^SI","^^SO","^^SP"},
                       {"^^SA","^^SS","^^SD","^^SF","^^SG","^^SH","^^SJ","^^SK","^^SL","^^^e"},
                       {"^^SZ","^^SX","^^SC","^^SV","^^SB","^^SN","^^SM","^^n,","^^n.","^^^S"},
                       {"^^^p","^^^x","U00BF","U00A1","U00E8","U00E0","U00F2","U00EC","U00F9","^^^s"},
                       {"U00E4","U00F6","U00FC","U00F1","U00E9","U00E1","U00F3","U00ED","U00FA","U00DF"}};
// default vertical offset
int ofs = 33;
// buttons ratio
float ra = 1;
// ---

static void spage(short p);
static void upage();
static void npage();
static void ppage();
// Handle the shift status and color
static void shiftb() {
switch(sbt) {
case 0:
sbt = 1;
upage();
gtk_widget_modify_bg(bts, GTK_STATE_NORMAL, &sac);
gtk_widget_modify_bg(bts, GTK_STATE_PRELIGHT, &sac);
break;
case 1: //once
sbt = 2;
gtk_widget_modify_bg(bts, GTK_STATE_NORMAL, &sfc);
gtk_widget_modify_bg(bts, GTK_STATE_PRELIGHT, &sfc);
break;
case 2: //locked
sbt = 0;
spage(1);
break; } }
// Remap and sends the key
static bool seec( char* d ) {
sym = XStringToKeysym((char *)d);
if ( sym < 160 ) {
printf("No remap.\n");
return false; }
KeySym ksl[2] = { sym, sym };
// remap the keycode 249
XChangeKeyboardMapping(dpy, 249, 2, ksl, 1);
XTestFakeKeyEvent(dpy, 249, True, 0);
XTestFakeKeyEvent(dpy, 249, False, 0);
XSync(dpy, False);
XTestGrabControl(dpy, False);
if (sbt == 1) { sbt = 0; spage(1); }
return true;
}
// Sends the key
static void sec( GtkWidget *w, char* d) {
ln = strlen(d);
if (ln) {
XTestGrabControl(dpy, True);
if (ln > 3 && d[0] == '^' && d[1] == '^') { //prefix
strncpy(tc, d+3, ln-3);
tc[ln-3] = 0;
if (d[2] == '^') { 
switch(d[3]) {
case 's': sym = XK_space; break;
case 'x': sym = XK_BackSpace; break;
case 'e': sym = XK_Return; break;
case 'p': npage(); return;
case 'P': ppage(); return;
case 'S': shiftb(); return;
case 'u': sym = XK_Up; break;
case 'd': sym = XK_Down; break;
case 'l': sym = XK_Left; break;
case 'r': sym = XK_Right; break;
case 't': sym = XK_Tab; break;
} } else if (d[2] == 'n') {
sym = d[3];
} else if (d[2] == 's') {
sym = d[3];
shi = 1;
} else if (d[2] == 'S') {
sym = XStringToKeysym(tc);
shi = 1; } }
else if (ln >= 5 && d[0] == 'U') { //unicode
if (seec(d)) { return; } }
else { sym = XStringToKeysym((char *)d); } //regular
if (sym == NoSymbol) printf("Keysym error.\n");
if (shi) XTestFakeKeyEvent(dpy, shic, True, 0); //shift
code = XKeysymToKeycode(dpy, sym);
if (code == 0) { printf("Keycode error.\n"); return; }
XTestFakeKeyEvent(dpy, code, True, 0);
XTestFakeKeyEvent(dpy, code, False, 0);
if (shi) { XTestFakeKeyEvent(dpy, shic, False, 0); shi = 0; }
XSync(dpy, False);
XTestGrabControl(dpy, False);
if (sbt == 1) { sbt = 0; spage(1); } } }
// Returns the label for the special buttons
char* rlbl( char c ) {
ws = 0;
switch(c) {
case 'e': return "↵";
case 's': return "␣";
case 'x': return "⌫";
case 'S': ws = 1; return "⇧";
case 'p': return "▶";
case 'P': return "◀";
case 'u': return "↑";
case 'd': return "↓";
case 'l': return "←";
case 'r': return "→";
case 't': return "⇥";
default: return ""; } }
// Shows the page n. p rebuilding the table
static void spage( short p ) {
// clear the table
if (ei) gtk_container_foreach(GTK_CONTAINER(table), (GtkCallback)gtk_widget_destroy, NULL);
short x, y, n, le;
short rt = nr * p;
short yd = nr * ( p - 1 );
char k[5] = {0,0,0,0,0};
sbt = 0;
// loop the keys table array and add buttons
for (y=0; yd < rt; ++y) {
for (x=0; x < nc; ++x) {
le = strlen(keya[yd][x]);
if (le) {
if (le > 3 && keya[yd][x][0] == '^' && keya[yd][x][1] == '^') {
k[0] = keya[yd][x][3]; k[1]=0;
if (keya[yd][x][2] == '^') {
bt = gtk_button_new_with_label(rlbl(k[0]));
if (ws) { bts = bt; } }
else { bt = gtk_button_new_with_label(k); } }
else if (le >= 5 && keya[yd][x][0] == 'U') { // unicode
sscanf(keya[yd][x], "U%x", &co);
n = wcrtomb(k, (wchar_t)co, NULL);
k[n] = 0;
bt = gtk_button_new_with_label(k); }
else { bt = gtk_button_new_with_label(keya[yd][x]); }
if (fl) gtk_button_set_relief(GTK_BUTTON(bt), GTK_RELIEF_NONE);
g_signal_connect(bt, "clicked", G_CALLBACK(sec), &keya[yd][x]);
gtk_table_attach_defaults(GTK_TABLE(table), bt, x, x+1, y, y+1); } }
++yd; }
ei = 1;
gtk_widget_show_all(table); }
// Shows the up-case page rebuilding the table
static void upage() {
gtk_container_foreach(GTK_CONTAINER(table), (GtkCallback)gtk_widget_destroy, NULL);
short x, y, n, le;
char k[5] = {0,0,0,0,0};
// loop the upcase keys table array and add buttons
for (y=0; y < nr; ++y) {
for (x=0; x < nc; ++x) {
le = strlen(ukey[y][x]);
if (le) {
if (le > 3 && ukey[y][x][0] == '^' && ukey[y][x][1] == '^') {
k[0] = ukey[y][x][3];
if (keya[y][x][2] == '^') {
bt = gtk_button_new_with_label(rlbl(k[0]));
if (ws) { bts = bt; } }
else { bt = gtk_button_new_with_label(k); } }
else if (le == 5 && ukey[y][x][0] == 'U') {
sscanf(ukey[y][x], "U%x", &co);
n = wcrtomb(k, (wchar_t)co, NULL);
k[n] = 0;
bt = gtk_button_new_with_label(k);
} else { bt = gtk_button_new_with_label(ukey[y][x]); }
if (fl) gtk_button_set_relief(GTK_BUTTON(bt), GTK_RELIEF_NONE);
g_signal_connect(bt, "clicked", G_CALLBACK(sec), &ukey[y][x]);
gtk_table_attach_defaults(GTK_TABLE(table), bt, x, x+1, y, y+1); } } }
gtk_widget_show_all(table); }
// Next page
static void npage() {
if (++pg > tp) pg = 1;
spage(pg); }
// Prev page
static void ppage() {
if (--pg < 1) pg = tp;
spage(pg); }
// Quiting
static gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data ) {
if (nkc) { //set former layout
char kc[20] = "setxkbmap ";
strcat(kc, ly);
system(kc);
}
gtk_main_quit();
return FALSE; }
// Main
int main( int argc, char *argv[] ) {
gtk_init(&argc, &argv);
bool esp = 0, scs = 0, ko = 0;
int sh = -1, sw = -1, wh = -1, ww = -1;
GtkWindowType wt = GTK_WINDOW_TOPLEVEL;
GtkAllocation* alc = g_new(GtkAllocation, 1);
gdk_color_parse("#00B2CF", &sac);
gdk_color_parse("#FF351F", &sfc);
// arguments
if (argc) {
bool ws = 0, hs = 0;
int wo = 0, ho = 0, oo = 0;
for (int i=1; i < argc; ++i) {
if (!strcmp(argv[i], "-e")) { esp = 1; }
else if (!strcmp(argv[i], "-k")) { ko = 1; }
else if (!strcmp(argv[i], "-w")) {
if (i + 1 < argc && isdigit(argv[i+1][0]) && strlen(argv[i+1]) < 6) {
wo = atol(argv[i+1]);
if (wo) { ws = 1; ++i; } } }
else if (!strcmp(argv[i], "-h")) {
if (i + 1 < argc && isdigit(argv[i+1][0]) && strlen(argv[i+1]) < 6) {
ho = atol(argv[i+1]);
if (ho) { hs = 1; ++i; } } }
else if (!strcmp(argv[i], "-o")) {
if (i + 1 < argc && strlen(argv[i+1]) < 5) {
oo = atol(argv[i+1]);
if (oo != 0) { ofs = oo; ++i; } } }
else if (!strcmp(argv[i], "-t")) { gtk_rc_parse(".ok.rc"); }
else if (!strcmp(argv[i], "-f")) { fl = 1; }
else if (!strcmp(argv[i], "-p")) { wt = GTK_WINDOW_POPUP; }
else if (!strcmp(argv[i], "-?")) { printf("\nOnKey 0.5 on-screen keyboard.\n\nOPTIONS:\n-e	Expand to screen width\n-k	Temporary set us keyboard layout\n-w	Width\n-h	Height\n-o	Vertical position offset\n-t	Load a gtk style at '~/.ok.rc'\n-f	Flat buttons\n-p	Visible on fullscreen apps\n\n"); exit(0); }
}
if (hs) {
wh = ho;
if (ws) { ww = wo; esp = 0; }
} else {
if (ws) {
ww = wo;
esp = 0;
wh = ww / 3;
} } }
sh  = gdk_screen_height();
sw  = gdk_screen_width();
if (ko) {
// read keyboard layout
char lya[150];
FILE* pi = popen("setxkbmap -query", "r");
if (pi != NULL) {
int p = 0, ll = 0, li = 0;
ll = fread(lya, 1, 150, pi);
pclose(pi);
char *lys = strstr(lya, "layout:");
p = lys - lya;
p+=7;
for (; p < ll; ++p) {
if (lya[p] > 32) {
ly[li] = lya[p];
++li; } }
ly[li] = 0;
if (strcmp(ly, "us") != 0) {
// set us keyboard layout
system("setxkbmap us");
nkc = 1; } } }
// window size
if ( sh > 9 && sw > 9 ) scs = 1;
if (esp && scs) {
ww = sw;
if (wh < 1) { wh = (ww / nc) * ra * nr; } }
dpy = XOpenDisplay(NULL);
if (!dpy) printf("X display error!\n");
shic = XKeysymToKeycode(dpy, XK_Shift_L);
// ui setup
win = gtk_window_new(wt);
gtk_window_set_accept_focus(GTK_WINDOW(win), 0);
gtk_window_set_focus_on_map(GTK_WINDOW(win), 0);
gtk_widget_set_can_focus(win, 0);
gtk_window_set_title(GTK_WINDOW(win), "OnKey");
gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
gtk_widget_set_size_request(win, ww, wh);
gtk_window_stick(GTK_WINDOW(win));
gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);
g_signal_connect(win, "delete-event", G_CALLBACK(delete_event), NULL);
table = gtk_table_new(nr, nc, TRUE);
gtk_container_add(GTK_CONTAINER(win), table);
spage(1);
gtk_widget_show(win);
// move the window to center-bottom if...
gtk_widget_get_allocation(GTK_WIDGET(win), alc);
if (wh < 1) wh = alc->height;
if (ww < 1) ww = alc->width;
if (ww && wh && scs) gtk_window_move(GTK_WINDOW(win), (sw/2)-(ww/2), sh-wh-ofs);
XFlush(dpy);
gtk_main();
return 0;
}
