#include <graphics.h>

int main() {
    // 1. »«“ ò—œ‰ Ìò Å‰Ã—Â ê—«›ÌòÌ »« «»⁄«œ 600 œ— 400
    initwindow(600, 400, "Dev-C++ Graphics Test");

    // 2. ‰Ê‘ ‰ „ ‰
    setcolor(WHITE);
    outtextxy(180, 80, (char*)"Graphics is Working Successfully!");

    // 3. —”„ Ìò œ«Ì—Â ”»“  ÊÅ—
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    fillellipse(300, 200, 50, 50);

    // 4. —”„ Ìò Œÿ ÷ŒÌ„ ›Ì—Ê“Âù«Ì
    setcolor(CYAN);
    setlinestyle(SOLID_LINE, 0, 3);
    line(150, 300, 450, 300);

    // „‰ Ÿ— „«‰œ‰ »—«Ì ›‘—œ‰ Ìò ò·Ìœ ÃÂ  »” ‰ Å‰Ã—Â
    getch();
    closegraph();
    return 0;
}
