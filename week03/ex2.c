#include <stdio.h>
#include <math.h>
#include <stdlib.h>
struct Point{
    double x;
    double y;
};
double distance(struct Point point1, struct Point point2){
    return sqrtf(pow((point1.x-point2.x),2)+pow((point1.y-point2.y),2));
}
struct Triangle{
    struct Point point1;
    struct Point point2;
    struct Point point3;
};
double area(struct Triangle triangle1){
    return 0.5*(abs(triangle1.point1.x*triangle1.point2.y - triangle1.point2.x*triangle1.point1.y
                + triangle1.point2.x*triangle1.point3.y - triangle1.point3.x*triangle1.point2.y
                + triangle1.point3.x*triangle1.point1.y - triangle1.point1.x*triangle1.point3.y));
}

int main(void){
    struct Point point1;
    struct Point point2;
    struct Point point3;
    point1.x=2.5; point1.y=6;
    point2.x=1;point2.y=2.2;
    point3.x=10;point3.y=6;
    struct Triangle triangle1;
    triangle1.point1=point1;
    triangle1.point2=point2;
    triangle1.point3=point3;
    
    printf("%f %f", distance(point1,point2),area(triangle1));
}