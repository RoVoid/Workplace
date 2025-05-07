#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x, y;
} Point;

int main() {
    unsigned n;
    Point* points, target;

    FILE* file = fopen("1-8-11in.txt", "r");
    if (!file) return 1;

    fscanf(file, "%f%f%u", &target.x, &target.y, &n);
    points = (Point*)malloc(n * sizeof(Point));
    for (unsigned i = 0; i < n; i++) {
        points[i] = (Point){ 0, 0 };
        fscanf(file, "%f%f", &points[i].x, &points[i].y);
    }

    fclose(file);

    char inside = 0;
    for (unsigned i = 0; i < n; i++) {
        Point p0 = points[i], p1 = points[(i + 1) % n];
        if (p0.x < target.x && p1.x < target.x) continue;
        if ((p0.y > target.y) != (p1.y > target.y)) {
            if (target.x < (p1.x - p0.x) * (target.y - p0.y) / (p1.y - p0.y) + p0.x) inside = !inside;
        }
    }

    printf(inside ? "YES... UwU" : "NO... TvT");

    free(points);
    return 0;
}