#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <stdbool.h>

#define MAX_TITLES 256
#define LINE_H 18
#define LIST_TOP 56
#define LIST_BOTTOM 220

typedef struct {
    u64 id;
    u16 version;
    char name[96];
    bool favorite;
} NovaTitle;

static NovaTitle titles[MAX_TITLES];
static int titleCount = 0;
static int selected = 0;
static int scroll = 0;
static bool showInfo = false;
static bool searchMode = false;
static char searchText[64] = "";
static bool english = false;

static void drawText(int x, int y, const char *s, u32 color) {
    // PrintConsole is used for reliable baseline text rendering.
    (void)x; (void)y; (void)s; (void)color;
}

static int cmpTitle(const void *a, const void *b) {
    const NovaTitle *ta = (const NovaTitle *)a;
    const NovaTitle *tb = (const NovaTitle *)b;
    return strcasecmp(ta->name, tb->name);
}

static bool containsIgnoreCase(const char *haystack, const char *needle) {
    if (!needle[0]) return true;
    char a[128], b[64];
    snprintf(a, sizeof(a), "%s", haystack);
    snprintf(b, sizeof(b), "%s", needle);
    for (char *p=a; *p; ++p) if (*p >= 'A' && *p <= 'Z') *p += 32;
    for (char *p=b; *p; ++p) if (*p >= 'A' && *p <= 'Z') *p += 32;
    return strstr(a, b) != NULL;
}

static void loadFavorites(void) {
    FILE *f = fopen("sdmc:/3ds/NOVA_3DS/favorites.txt", "r");
    if (!f) return;
    unsigned long long id;
    while (fscanf(f, "%llx", &id) == 1) {
        for (int i=0; i<titleCount; ++i)
            if (titles[i].id == (u64)id) titles[i].favorite = true;
    }
    fclose(f);
}

static void saveFavorites(void) {
    mkdir("sdmc:/3ds/NOVA_3DS", 0777);
    FILE *f = fopen("sdmc:/3ds/NOVA_3DS/favorites.txt", "w");
    if (!f) return;
    for (int i=0; i<titleCount; ++i)
        if (titles[i].favorite) fprintf(f, "%016llx\n", (unsigned long long)titles[i].id);
    fclose(f);
}

static void scanInstalled(void) {
    titleCount = 0;
    u32 count = 0;
    if (R_FAILED(AM_GetTitleCount(MEDIATYPE_SD, &count))) return;
    if (count > MAX_TITLES) count = MAX_TITLES;
    u64 ids[MAX_TITLES];
    u16 versions[MAX_TITLES];
    u32 readCount = 0;
    if (R_FAILED(AM_GetTitleList(&readCount, MEDIATYPE_SD, count, ids))) return;
    for (u32 i=0; i<readCount && i<MAX_TITLES; ++i) {
        titles[titleCount].id = ids[i];
        titles[titleCount].version = 0;
        titles[titleCount].favorite = false;
        snprintf(titles[titleCount].name, sizeof(titles[titleCount].name),
                 "Titel %016llX", (unsigned long long)ids[i]);
        titleCount++;
    }
    qsort(titles, titleCount, sizeof(NovaTitle), cmpTitle);
    loadFavorites();
}

static int visibleIndex(int n) {
    int seen = 0;
    for (int i=0; i<titleCount; ++i) {
        if (!containsIgnoreCase(titles[i].name, searchText)) continue;
        if (seen++ == n) return i;
    }
    return -1;
}

static int visibleCount(void) {
    int n=0;
    for (int i=0; i<titleCount; ++i)
        if (containsIgnoreCase(titles[i].name, searchText)) n++;
    return n;
}

static void launchSelected(void) {
    // Launching titles is intentionally not wired in this first prototype.
    // The title browser is read-only until the launch flow is validated on-device.
}

static void render(void) {
    consoleClear();
    printf("\x1b[36m");
    printf("  ███╗   ██╗ ██████╗ ██╗   ██╗ █████╗\n");
    printf("  ████╗  ██║██╔═══██╗██║   ██║██╔══██╗\n");
    printf("  ██╔██╗ ██║██║   ██║██║   ██║███████║\n");
    printf("  ██║╚██╗██║██║   ██║╚██╗ ██╔╝██╔══██║\n");
    printf("  ██║ ╚████║╚██████╔╝ ╚████╔╝ ██║  ██║\n");
    printf("  ╚═╝  ╚═══╝ ╚═════╝   ╚═══╝  ╚═╝  ╚═╝\n");
    printf("\x1b[0m");
    printf("  NOVA 3DS  |  %s\n", english ? "HOME" : "START");
    printf("  ------------------------------------------------\n");
    if (showInfo) {
        printf("\n  %s\n", english ? "ABOUT NOVA 3DS" : "UEBER NOVA 3DS");
        printf("\n  %s\n", english ? "Homebrew launcher prototype" : "Homebrew-Launcher-Prototyp");
        printf("  %s: %d\n", english ? "Installed SD titles" : "Installierte SD-Titel", titleCount);
        printf("\n  A/B: %s\n", english ? "Back" : "Zurueck");
        printf("  X: %s\n", english ? "Search" : "Suche");
        printf("  Y: %s\n", english ? "Sort by name" : "Nach Namen sortieren");
        printf("  L: %s\n", english ? "Favorite" : "Favorit");
        return;
    }
    printf("  %s: %d   %s: %s\n",
           english ? "GAMES" : "SPIELE", visibleCount(),
           english ? "SEARCH" : "SUCHE", searchText[0] ? searchText : "-");
    printf("  ------------------------------------------------\n");
    int total = visibleCount();
    if (total == 0) printf("\n  %s\n", english ? "No titles found." : "Keine Titel gefunden.");
    for (int row=0; row<8; ++row) {
        int idx = visibleIndex(scroll + row);
        if (idx < 0) break;
        if (scroll + row == selected) printf("\x1b[47;30m");
        else printf("\x1b[0m");
        printf("  %c %-34.34s %s\n",
               scroll+row == selected ? '>' : ' ',
               titles[idx].name,
               titles[idx].favorite ? "*" : "");
        printf("\x1b[0m");
    }
    printf("\n  ------------------------------------------------\n");
    printf("  A: %s   X: %s   Y: %s\n",
           english ? "Launch" : "Starten",
           english ? "Search" : "Suche",
           english ? "Sort" : "Sortieren");
    printf("  L: %s   R: %s   START: %s\n",
           english ? "Favorite" : "Favorit",
           english ? "Info" : "Info",
           english ? "Exit" : "Beenden");
}

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    romfsInit();
    amInit();
    aptInit();
    cfguInit();

    scanInstalled();
    bool running = true;
    while (aptMainLoop() && running) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        int total = visibleCount();

        if (kDown & KEY_START) running = false;
        if (kDown & KEY_R) showInfo = !showInfo;
        if (showInfo && (kDown & KEY_B)) showInfo = false;

        if (!showInfo) {
            if (kDown & KEY_DOWN) { if (selected < total-1) selected++; }
            if (kDown & KEY_UP) { if (selected > 0) selected--; }
            if (selected < scroll) scroll = selected;
            if (selected >= scroll+8) scroll = selected-7;
            if (kDown & KEY_A) launchSelected();
            if (kDown & KEY_Y) qsort(titles, titleCount, sizeof(NovaTitle), cmpTitle);
            if (kDown & KEY_L) {
                int idx = visibleIndex(selected);
                if (idx >= 0) { titles[idx].favorite = !titles[idx].favorite; saveFavorites(); }
            }
            if (kDown & KEY_X) {
                SwkbdState swkbd;
                swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
                swkbdSetHintText(&swkbd, english ? "Search titles" : "Titel suchen");
                char input[64] = "";
                (void)swkbdInputText(&swkbd, input, sizeof(input));
                snprintf(searchText, sizeof(searchText), "%s", input);
                selected = scroll = 0;
            }
        }
        render();
        gspWaitForVBlank();
    }

    saveFavorites();
    cfguExit();
    aptExit();
    amExit();
    romfsExit();
    gfxExit();
    return 0;
}
