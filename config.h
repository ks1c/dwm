/* See LICENSE file for copyright and license details. */
/* appearance */
#include <X11/XF86keysym.h>
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappx     = 18;       /* gap pixel between windows */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12" };
static const char dmenufont[]       = "monospace:size=12";

/* DRACULA COLORS */
static const char nbg[]             = "#21222c";
static const char nfg[]             = "#f8f8f2";
static const char nbo[]             = "#21222c";
static const char sbg[]             = "#bd93f9";
static const char sfg[]             = "#21222c";
static const char sbo[]             = "#bd93f9";

static const char *colors[][3]      = {
	[SchemeNorm] = { nfg, nbg, nbo },
	[SchemeSel]  = { sfg, sbg, sbo },
};

/* GRUVBOX COLORS
static const char nbg[]             = "#1d2021";
static const char nfg[]             = "#bdae92";
static const char nbo[]             = "#1d2021";
static const char sbg[]             = "#32302f";
static const char sfg[]             = "#bdae92";
static const char sbo[]             = "#bdae93";

static const char *colors[][3]      = {
	[SchemeNorm] = { nfg, nbg, nbo },
	[SchemeSel]  = { sfg, sbg, sbo },
}; */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

/* draw tag line */
static const int drwtagline = 0;
static const int lnthic     = 2;

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class              instance    title       tags mask     isfloating   monitor */
	{ "TelegramDesktop",  NULL,       NULL,       0,            1,           -1 },
	{ "Gimp",             NULL,       NULL,       0,            1,           -1 },
	{ "firefox",          NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Thunderbird",      NULL,       NULL,       1 << 7,       0,           -1 },
	{ "Steam",            NULL,       "Steam",    1 << 6,       0,           -1 },
	{ "jetbrains-studio", NULL,       NULL,       1 << 0,       0,           -1 },
	{ "Gdx2D",            NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", nbg, "-nf", nfg, "-sb", sbg, "-sf", sfg, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *ssselecteditcmd[]  = { "screenshot", "selection-edit", NULL };
static const char *ssselectioncmd[]  = { "screenshot", "selection", NULL };
static const char *ssfullscreencmd[]  = { "screenshot", "fullscreen", NULL };
static const char *powermenucmd[]  = { "powermenu", NULL };
static const char *vifmcmd[]  = { "st", "-e", "vifmub", NULL };
static const char *raisevolcmd[]  = { "volume", "raise", NULL };
static const char *lowervolcmd[]  = { "volume", "lower", NULL };
static const char *mutevolcmd[]  = { "volume", "mute", NULL };
static const char *maxvolcmd[]  = { "volume", "max", NULL };
static const char *launchtelegramcmd[]  = { "launchtelegram", NULL };
static const char *htopcmd[]  = { "st", "-e", "htop", NULL };
static const char *refreshbarcmd[]  = { "dwmbar", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */

	// General key bindings
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_Escape, spawn,          {.v = powermenucmd } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Print,  spawn,          {.v = ssselectioncmd } },
	{ MODKEY|ShiftMask,             XK_Print,  spawn,          {.v = ssselecteditcmd } },
	{ 0,                            XK_Print,  spawn,          {.v = ssfullscreencmd } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	// Media key bindings
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = raisevolcmd } },
	{ MODKEY,                       XF86XK_AudioRaiseVolume,  spawn,          {.v = maxvolcmd } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = lowervolcmd } },
	{ MODKEY,                       XF86XK_AudioLowerVolume,  spawn,          {.v = mutevolcmd } },

	// Letter key bindings
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_w,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = vifmcmd } },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          {.v = launchtelegramcmd } },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_h,      spawn,          {.v = htopcmd } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,	                XK_b,      spawn,          {.v = refreshbarcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },

	// Number key bindings
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

