#include <iostream>
#include "defs.h"
#include "graphics.h"
#include "car.h"
#include "obstacle.h"

using namespace std;

int main(int argc, char* argv[]) {
    Graphics graphics;
    graphics.init(); // Khởi tạo đồ họa và tài nguyên

    while (graphics.running) {
        graphics.state();        // Xử lí trạng thái
        graphics.update();       // Cập nhật trạng thái game
        graphics.render();       // Vẽ
        SDL_Delay(16);           // 60 fps
    }

    graphics.quit();           // Giải phóng tài nguyên
    cout << "Game Over! Your score: " << graphics.score << endl;
    return 0;
}
