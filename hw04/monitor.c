#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <curses.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <ctype.h>

// Function prototypes
void handle_exit(int sig);
void draw_dashboard();
void resize_window();
void get_cpu_usage(double *cpu_usage);
void get_memory_usage(long *total_mem, long *used_mem, long *swap_used);
int get_process_count();
void get_process_states(int *running, int *sleeping, int *stopped, int *zombie);
void draw_bar_chart(WINDOW *win, int y, int x, int width, int running, int sleeping, int stopped, int zombie);
void draw_cpu_visualization(WINDOW *win, int y, int x, double cpu_usage);
void draw_memory_visualization(WINDOW *win, int y, int x, long total_mem, long used_mem);

// Global variable
WINDOW *win;

int main() {
    // Ctrl+C 핸들러 설정
    signal(SIGINT, handle_exit);

    // curses 초기화
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    // 초기 창 생성
    resize_window();

    while (1) {
        draw_dashboard();
        sleep(1); // 1초마다 갱신
    }

    return 0;
}

// Ctrl+C 입력 시 종료 처리
void handle_exit(int sig) {
    endwin();
    printf("프로그램이 정상적으로 종료되었습니다.\n");
    exit(0);
}

// 창 크기를 동적으로 재설정하는 함수
void resize_window() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // 터미널 창 크기 확인

    // 기존 창 제거 후 새 창 생성
    if (win) {
        delwin(win);
    }
    win = newwin(max_y, max_x, 0, 0); // 동적 크기의 창 생성
}

// 대시보드를 출력하는 함수
void draw_dashboard() {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x); // 터미널 창 크기 확인

    wclear(win);    // 창 내용 지우기
    box(win, 0, 0); // 창 테두리 그리기

    // 섹션의 크기와 좌표 설정
    int left_width = (max_x * 4) / 10;      // 왼쪽 섹션 너비 (전체의 40%)
    int tab_space = 4;                     // 탭 간격 (공백 크기)
    int right_start_x = left_width + tab_space; // 오른쪽 섹션 시작 X 좌표
    int bottom_start = max_y / 2;          // 프로세스 섹션 시작 Y 좌표

    // CPU 관련 데이터
    double cpu_usage = 0.0;
    get_cpu_usage(&cpu_usage);
    int thread_count = (int)sysconf(_SC_NPROCESSORS_ONLN);

    mvwprintw(win, 1, 2, "[CPU]");
    mvwprintw(win, 2, 2, "CPU Usage: %.2f%%", cpu_usage);
    mvwprintw(win, 3, 2, "Threads: %d", thread_count);

    // CPU 시각화 그래프
    draw_cpu_visualization(win, 4, 2, cpu_usage);

    // 메모리 관련 데이터
    long total_mem = 0, used_mem = 0, swap_used = 0;
    get_memory_usage(&total_mem, &used_mem, &swap_used);

    double mem_usage_percent = (double)used_mem / total_mem * 100; // 퍼센트 계산
    mvwprintw(win, 1, right_start_x + 2, "[MEMORY]");
    mvwprintw(win, 2, right_start_x + 2, "Used Memory: %ld MB (%.2f%%)", used_mem / 1024, mem_usage_percent);
    mvwprintw(win, 3, right_start_x + 2, "Swap Usage: %ld MB", swap_used / 1024);

    // 메모리 시각화 그래프
    draw_memory_visualization(win, 4, right_start_x + 2, total_mem, used_mem);

    // 프로세스 관련 데이터
    int running = 0, sleeping = 0, stopped = 0, zombie = 0;
    get_process_states(&running, &sleeping, &stopped, &zombie);
    int total_processes = running + sleeping + stopped + zombie;

    mvwprintw(win, bottom_start, 2, "[PROCESSES]");
    mvwprintw(win, bottom_start + 1, 2, "Total: %d", total_processes);
    mvwprintw(win, bottom_start + 2, 2, "Running: %d", running);
    mvwprintw(win, bottom_start + 3, 2, "Sleeping: %d", sleeping);
    mvwprintw(win, bottom_start + 4, 2, "Stopped: %d", stopped);
    mvwprintw(win, bottom_start + 5, 2, "Zombie: %d", zombie);

    // 프로세스 상태 막대 그래프 출력
    mvwprintw(win, bottom_start, right_start_x + 2, "[GRAPH]");
    draw_bar_chart(win, bottom_start + 2, right_start_x + 2, max_x - right_start_x - 2, running, sleeping, stopped, zombie);

    wrefresh(win); // curses 화면 갱신
}




void draw_bar_chart(WINDOW *win, int y, int x, int width, int running, int sleeping, int stopped, int zombie) {
    int max_height = 13;                // 막대의 최대 높이를 10으로 설정 (기존보다 줄임)
    int bar_width = width / 4 - 2;      // 막대 하나의 너비 (가로 공간 4등분)
    int max_value = running > sleeping ? (running > stopped ? (running > zombie ? running : zombie) : (stopped > zombie ? stopped : zombie)) : (sleeping > stopped ? (sleeping > zombie ? sleeping : zombie) : (stopped > zombie ? stopped : zombie));
    
    if (max_value == 0) max_value = 1; // 최대값이 0인 경우 1로 설정하여 나눗셈 방지

    // 상태 값에 따른 막대 높이 계산 (값을 축소하여 출력)
    int running_height = (running * max_height) / (max_value + 10); // 최대값을 늘려 상대적 축소
    int sleeping_height = (sleeping * max_height) / (max_value + 10);
    int stopped_height = (stopped * max_height) / (max_value + 10);
    int zombie_height = (zombie * max_height) / (max_value + 10);

    // 막대 출력
    for (int i = 0; i < max_height; i++) {
        // Running 막대
        if (i < running_height) {
            mvwprintw(win, y + max_height - i, x, "|");
        }

        // Sleeping 막대
        if (i < sleeping_height) {
            mvwprintw(win, y + max_height - i, x + bar_width + 3, "|");
        }

        // Stopped 막대
        if (i < stopped_height) {
            mvwprintw(win, y + max_height - i, x + 2 * (bar_width + 3), "|");
        }

        // Zombie 막대
        if (i < zombie_height) {
            mvwprintw(win, y + max_height - i, x + 3 * (bar_width + 3), "|");
        }
    }

    // 상태 이름 출력 (가로축)
    mvwprintw(win, y + max_height + 1, x, "Running");
    mvwprintw(win, y + max_height + 1, x + bar_width + 3, "Sleeping");
    mvwprintw(win, y + max_height + 1, x + 2 * (bar_width + 3), "Stopped");
    mvwprintw(win, y + max_height + 1, x + 3 * (bar_width + 3), "Zombie");
}

// CPU 시각화
void draw_cpu_visualization(WINDOW *win, int y, int x, double cpu_usage) {
    mvwprintw(win, y, x, "Usage: [");
    int total_bars = 50; // 그래프의 총 길이 (칸 수: 50)
    int used_bars = (int)(cpu_usage / 100 * total_bars); // 사용 중인 부분 계산

    for (int i = 0; i < total_bars; i++) {
        if (i < used_bars) {
            waddch(win, 'o'); // 사용 중인 부분
        } else {
            waddch(win, 'x'); // 사용되지 않은 부분
        }
    }
    waddch(win, ']');
}

// 메모리 시각화
void draw_memory_visualization(WINDOW *win, int y, int x, long total_mem, long used_mem) {
    mvwprintw(win, y, x, "Memory: [");
    int total_bars = 50; // 그래프의 총 길이 (칸 수: 50)
    int used_bars = (int)((double)used_mem / total_mem * total_bars); // 사용 중인 부분 계산

    for (int i = 0; i < total_bars; i++) {
        if (i < used_bars) {
            waddch(win, 'o'); // 사용 중인 부분
        } else {
            waddch(win, 'x'); // 사용되지 않은 부분
        }
    }
    waddch(win, ']');
}





// CPU 사용량 계산
void get_cpu_usage(double *cpu_usage) {
    static long prev_idle = 0, prev_total = 0;
    long idle, total;
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Error opening /proc/stat");
        *cpu_usage = 0.0;
        return;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    fclose(file);

    long user, nice, system, idle_time, iowait, irq, softirq, steal;
    sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld %ld",
           &user, &nice, &system, &idle_time, &iowait, &irq, &softirq, &steal);

    idle = idle_time + iowait;
    total = user + nice + system + idle + iowait + irq + softirq + steal;

    long delta_idle = idle - prev_idle;
    long delta_total = total - prev_total;

    *cpu_usage = (100.0 * (delta_total - delta_idle) / delta_total);

    prev_idle = idle;
    prev_total = total;
}

// 메모리 사용량 계산
void get_memory_usage(long *total_mem, long *used_mem, long *swap_used) {
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        *total_mem = 0;
        *used_mem = 0;
        *swap_used = 0;
        return;
    }

    *total_mem = info.totalram;
    *used_mem = info.totalram - info.freeram;
    *swap_used = info.totalswap - info.freeswap;
}

// 실행 중인 프로세스 상태를 가져오는 함수
void get_process_states(int *running, int *sleeping, int *stopped, int *zombie) {
    DIR *proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("opendir /proc");
        return;
    }

    struct dirent *entry;
    char path[512], state;
    FILE *file;

    while ((entry = readdir(proc_dir)) != NULL) {
        if (!isdigit(entry->d_name[0])) continue;

        snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
        file = fopen(path, "r");
        if (file == NULL) continue;

        while (fgets(path, sizeof(path), file)) {
            if (sscanf(path, "State: %c", &state) == 1) {
                if (state == 'R') (*running)++;
                else if (state == 'S') (*sleeping)++;
                else if (state == 'T') (*stopped)++;
                else if (state == 'Z') (*zombie)++;
                break;
            }
        }
        fclose(file);
    }
    closedir(proc_dir);
}
