#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 3;        /* border pixel of windows */
static const unsigned int gappx = 0;        /* gaps between windows */
static const unsigned int snap = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;     /* 0 means no systray */  
static const unsigned int gappih = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv = 10;       /* vert inner gap between windows */
static const unsigned int gappoh = 12;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps = 10;        /* 1 means no outer gap when there is only one window */
static const int showbar = 1;        /* 0 means no bar */
static const int topbar = 0;        /* 0 means bottom bar */
static const Bool viewontag = True;     /* Switch view on tag switch */
static const char *fonts[] = {"Ubuntu Mono Nerd Font:size=14"};
static const char dmenufont[] = "SourceCodePro Nerd Font Mono:size=16";
static const char col_gray1[] = "#222222";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#bbbbbb";
static const char col_gray4[] = "#ffffff";
static const char col_cyan[] = "#37474F";
//static const char col_border[]        = "#FF9933";
//static const char col_border[]        = "#42A5F5";
static const char col_border[] = "#CD661D";
//static const char col_border[]        = "#33FFFF";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3] = {
        /*               fg         bg         border   */
        [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
        [SchemeSel]  = {col_gray4, col_cyan, col_border},
        [SchemeHid]  = {col_cyan, col_gray1, col_border},
};
static const unsigned int alphas[][3] = {
        /*               fg      bg        border     */
        [SchemeNorm] = {OPAQUE, baralpha, borderalpha},
        [SchemeSel]  = {OPAQUE, baralpha, borderalpha},
};

/* tagging */
static const char *tags[] = {"1-ToDo", "2-Web", "3-Note", "4-Code", "5-Dev", "6-Mail", "7-IM", "8-Stream", "9-Temp"};

static const Rule rules[] = {
        /* xprop(1):
         *	WM_CLASS(STRING) = instance, class
         *	WM_NAME(STRING) = title
         */
        /* class                  instance    title       tags mask     isfloating   monitor */
        {"dida",                NULL, NULL, 1 << 0, 0, -1},
        {"Google-chrome",       NULL, NULL, 1 << 1, 0, -1},
        {"Microsoft-edge-dev",  NULL, NULL, 1 << 1, 0, -1},
        {"Microsoft-edge",      NULL, NULL, 1 << 1, 0, -1},
        {"Vivaldi-stable",      NULL, NULL, 1 << 1, 0, -1},
        {"jetbrains-idea",      NULL, NULL, 1 << 4, 0, -1},
        {"Code",                NULL, NULL, 1 << 3, 0, -1},
        {"obsidian",            NULL, NULL, 1 << 2, 0, -1},
        {"siyuan",              NULL, NULL, 1 << 2, 0, -1},
        {"RemNote",             NULL, NULL, 1 << 2, 0, -1},
        {"Emacs",               NULL, NULL, 1 << 2, 0, -1},
        {"Anki",                NULL, NULL, 1 << 5, 0, -1},
        {"Thunderbird",         NULL, NULL, 1 << 5, 0, -1},
        {"netease-cloud-music", NULL, NULL, 1 << 6, 0, -1},
        {"weixin",              NULL, NULL, 1 << 6, 0, -1},
        {NULL, "tim.exe",             NULL, 1 << 6, 0, -1},
        {"Joplin",              NULL, NULL, 1 << 8, 0, -1},
        {"Steam",               NULL, NULL, 1 << 8, 0, -1},
        {"TelegramDesktop",     NULL, NULL, 1 << 6, 0, -1},
        {"Dynalist",            NULL, NULL, 1 << 7, 0, -1},
        {"parsecd",             NULL, NULL, 1 << 7, 0, -1},
        {"xfreerdp",            NULL, NULL, 1 << 7, 0, -1},
        {"org.remmina.Remmina", NULL, NULL, 1 << 7, 0, -1},
        {"GoldenDict",          NULL, NULL, NULL,   1, -1},
        {"Zeal",                NULL, NULL, NULL,   1, -1},
        {NULL,                NULL, "Ozone X11", NULL,   1, -1},

};

/* layout(s) */
static const float mfact = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
        /* symbol     arrange function */
        {"Tile", tile},    /* first entry is default */
        {"><>", NULL},    /* no layout function means floating behavior */
        {"[M]",  monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define MODKEY1 Mod1Mask
#define TAGKEYS(KEY, TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = {"rofi", "-show", "combi", NULL};
static const char *termcmd[] = {"st", "-e", "tmux"};
static const char *browsercmd[] = {"google-chrome-stable", NULL};
static const char *emacscmd[] = {"emacs", NULL};
static const char *dictcmd[] = {"rofi", "-show", "dict" "-modi" "dict:~/.config/rofi/dict.sh", NULL};
static const char *search_open_application[] = {"rofi", "-show", "window", NULL};


static const char *wpcmd[] = {"/home/fan/bin/change_wallpaper.sh", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = {"st", "-t", scratchpadname, "-g", "200x58", "-e", "tmux", NULL};

static const char *screenshotcmd[] = {"flameshot", "gui", NULL};
static const char *shutdowncmd[] = {"/home/fan/bin/update_shutdown.sh", NULL};
static const char *rebootcmd[] = {"shutdown", "-r", "now", NULL};
static const char *suspendcmd[] = {"systemctl", "suspend", NULL};

static const char *mutecmd[] = {"pactl", "set-sink-mute", "0", "toggle", NULL};
static const char *volupcmd[] = {"pactl", "set-sink-volume", "0", "+5%", NULL};
static const char *voldowncmd[] = {"pactl", "set-sink-volume", "0", "-5%", NULL};

static const char *brupcmd[] = {"light", "-A", "10", NULL};
static const char *brdowncmd[] = {"light", "-U", "10", NULL};
static const char *clearpdfcmd[] = {"/home/fan/bin/copy_without_linebreaks.sh", NULL};
static const char *change_to_windows[] = {"ddcutil", "-d", "1", "setvcp", "60", "0x11", NULL};
static const char *change_to_linux[] = {"ddcutil", "-d", "1", "setvcp", "60", "0x0f", NULL};
static const char *blanketcmd[] = {"blanket", NULL};


//static const char *copycmd[] ={"xdotool","getactivewindow", "key", "ctrl+0xff63", NULL};
//static const char *pastecmd[] ={"xdotool","getactivewindow", "key","shift+0xff63", NULL};


static Key keys[] = {
        /* modifier            key                      function        argument */
        {MODKEY, XK_d, spawn, {.v = dictcmd}},
        //{ MODKEY,              XK_space,                spawn,          {.v = dmenucmd } },
        {MODKEY1, XK_space, spawn, {.v = dmenucmd}},
        {MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},
        {MODKEY, XK_g, spawn, {.v = browsercmd}},
        {MODKEY, XK_e, spawn, {.v = emacscmd}},
        {MODKEY, XK_b, spawn, {.v = blanketcmd}},
        {0, XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd}},
        {0, XF86XK_AudioMute, spawn, {.v = mutecmd}},
        {0, XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd}},
        {0, XF86XK_MonBrightnessUp, spawn, {.v = brupcmd}},
        {0, XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd}},

        {0, XK_Print, spawn, {.v = screenshotcmd}},
        {MODKEY | ShiftMask, XK_s, spawn, {.v = shutdowncmd}},
        {MODKEY, XK_s, spawn, {.v = suspendcmd}},
        {MODKEY | ShiftMask, XK_r, spawn, {.v = rebootcmd}},
        {MODKEY, XK_p, spawn, {.v =  wpcmd}},
        {MODKEY, XK_c, spawn, {.v =  clearpdfcmd}},
        {MODKEY1, XK_F1, spawn, {.v = change_to_windows}},

        {MODKEY, XK_h, setmfact, {.f = -0.05}},
        {MODKEY | ShiftMask, XK_h, tagtoleft, {0}},
        {MODKEY, XK_j, focusstack, {.i = +1}},
        {MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
        {MODKEY, XK_k, focusstack, {.i = -1}},
        {MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},
        {MODKEY, XK_l, setmfact, {.f = +0.05}},
        {MODKEY1, XK_F2, spawn, {.v = change_to_linux}},
        {MODKEY, XK_w, spawn, {.v =search_open_application}},

        {MODKEY, XK_u, hidewin, {0}},
        {MODKEY | ShiftMask, XK_u, restorewin, {0}},
        {MODKEY, XK_i, hideotherwins, {0}},
        {MODKEY | ShiftMask, XK_i, restoreotherwins, {0}},
        {MODKEY, XK_Tab, view, {0}},
        {MODKEY | ShiftMask, XK_q, killclient, {0}},
        {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
        {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
        {MODKEY, XK_f, fullscreen, {0}},
        //{ MODKEY,              XK_d,                setlayout,      {0} },
        {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
        {MODKEY, XK_Return, togglescratch, {.v = scratchpadcmd}},
        {MODKEY, XK_0, view, {.ui = ~0}},
        {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
        {MODKEY, XK_comma, focusmon, {.i = -1}},
        {MODKEY, XK_period, focusmon, {.i = +1}},
        {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
        {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
        TAGKEYS(XK_1, 0)
        TAGKEYS(XK_2, 1)
        TAGKEYS(XK_3, 2)
        TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4)
        TAGKEYS(XK_6, 5)
        TAGKEYS(XK_7, 6)
        TAGKEYS(XK_8, 7)
        TAGKEYS(XK_9, 8)
        {MODKEY | ControlMask, XK_q, quit, {0}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        {ClkLtSymbol,   0,     Button1, setlayout,      {0}},
        {ClkLtSymbol,   0,     Button3, setlayout,      {.v = &layouts[2]}},
        {ClkStatusText, 0,     Button2, spawn,          {.v = termcmd}},
        {ClkClientWin, MODKEY, Button1, movemouse,      {0}},
        {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
        {ClkClientWin, MODKEY, Button3, resizemouse,    {0}},
        {ClkTagBar,     0,     Button1, view,           {0}},
        {ClkTagBar,     0,     Button3, toggleview,     {0}},
        {ClkTagBar,    MODKEY, Button1, tag,            {0}},
        {ClkTagBar,    MODKEY, Button3, toggletag,      {0}}
};

