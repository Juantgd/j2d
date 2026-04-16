#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

// ====================== GUI 控件定义 ======================
// 按钮控件（核心）
typedef struct {
  float x, y;             // 坐标
  float w, h;             // 尺寸
  const char *text;       // 显示文字
  SDL_Color bg;           // 背景色
  SDL_Color hover_bg;     // 悬停背景色
  SDL_Color text_color;   // 文字颜色
  int is_hovered;         // 是否悬停
  void (*on_click)(void); // 点击回调函数
} Button;

// 文本标签
typedef struct {
  float x, y;
  const char *text;
  SDL_Color color;
} Label;

// ====================== 全局变量 ======================
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
int running = 1;

// ====================== GUI 渲染函数 ======================
// 渲染文字（通用）
void render_text(float x, float y, float width, float height, const char *text,
                 SDL_Color color) {
  if (!font || !text)
    return;

  SDL_Surface *surface =
      TTF_RenderText_Blended(font, text, strlen(text), color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  float text_width, text_height;
  SDL_GetTextureSize(texture, &text_width, &text_height);
  if (width != 0 || height != 0)
    x += (width - text_width) / 2;
  if (height != 0)
    y += (height - text_height) / 2;
  SDL_FRect dst = {x, y, text_width, text_height};
  // SDL_Log("texture: {x: %.2f,y: %.2f,w: %.2f,h: %.2f}\n", x, y, text_width,
  //         text_height);
  SDL_RenderTexture(renderer, texture, NULL, &dst);

  SDL_DestroyTexture(texture);
  SDL_DestroySurface(surface);
}

// 渲染按钮
void render_button(Button *btn) {
  // 选择颜色：悬停=高亮色，否则=普通色
  SDL_Color current_bg = btn->is_hovered ? btn->hover_bg : btn->bg;

  // 绘制按钮背景
  SDL_FRect rect = {btn->x, btn->y, btn->w, btn->h};
  SDL_SetRenderDrawColor(renderer, current_bg.r, current_bg.g, current_bg.b,
                         current_bg.a);
  SDL_RenderFillRect(renderer, &rect);

  // 绘制按钮边框
  SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
  SDL_RenderRect(renderer, &rect);

  // 文字居中
  render_text(btn->x, btn->y, btn->w, btn->h, btn->text, btn->text_color);
}

// 渲染标签
void render_label(Label *label) {
  render_text(label->x, label->y, 800, 0, label->text, label->color);
}

// ====================== GUI 事件处理 ======================
// 检测鼠标是否在按钮上
int is_point_in_rect(float px, float py, Button *btn) {
  return (px >= btn->x && px <= btn->x + btn->w && py >= btn->y &&
          py <= btn->y + btn->h);
}

// 更新按钮状态（悬停+点击）
void update_button(Button *btn, SDL_Event *event) {
  float mx, my;
  SDL_GetMouseState(&mx, &my);

  // 更新悬停状态
  btn->is_hovered = is_point_in_rect(mx, my, btn);

  // 检测鼠标左键点击
  if (event->type == SDL_EVENT_MOUSE_BUTTON_UP &&
      event->button.button == SDL_BUTTON_LEFT && btn->is_hovered &&
      btn->on_click) {
    btn->on_click(); // 执行点击逻辑
  }
}

// ====================== 按钮点击回调示例 ======================
void on_button_click() { SDL_Log("按钮被点击了！！！"); }

// ====================== 主函数 ======================
int main(int argc, char *argv[]) {
  // 1. 初始化 SDL
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  // 2. 创建窗口 + 渲染器
  SDL_CreateWindowAndRenderer("SDL3 GUI 示例", 800, 600, 0, &window, &renderer);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  // 3. 加载字体（替换为你的字体路径，如 msyh.ttc / Arial.ttf）
  font = TTF_OpenFont("/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc",
                      24);
  if (!font) {
    SDL_LogError(0, "字体加载失败: %s", SDL_GetError());
    return -1;
  }

  // 4. 创建 GUI 控件
  Button btn = {.x = 300,
                .y = 250,
                .w = 200,
                .h = 60,
                .text = "点击我",
                .bg = {100, 149, 237, 255},      //  cornflowerblue
                .hover_bg = {65, 105, 225, 255}, // 悬停色
                .text_color = {255, 0, 0, 255},
                .on_click = on_button_click};

  Label label = {.x = 0,
                 .y = 150,
                 .text = "SDL3 手动 GUI 演示",
                 .color = {255, 255, 0, 255}};

  // 5. 主循环
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      // 退出窗口
      if (event.type == SDL_EVENT_QUIT)
        running = 0;

      // 更新按钮事件
      update_button(&btn, &event);
    }

    // 清空屏幕
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // 渲染 GUI
    render_label(&label);
    render_button(&btn);

    // 显示画面
    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 FPS
  }

  // 6. 释放资源
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
