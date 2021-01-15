/**
 * 迷宫 源文件
 * 第一版实现
*/

#include <iostream>
#include "./maze.h"

using namespace std;

int main(){
    // 简易6阶迷宫
    int mazeMatrix[][MaxRank] = {
        {1, 1, 1, 1, 1, 1},
	    {0, 0, 0, 0, 1, 1},
        {1, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1},
	};
    position entrance = { 1, 0, -2},
             exit = {4, 5, -2};
             
    Maze maze = Maze(mazeMatrix, 6, entrance, exit);
    
    maze.printMazeMeta();
    maze.printMaze(); // 先打印看看结果

    maze.findSolution(); // 查找一条通路
    maze.setAccess(); // 将结果写到地图数据中
    maze.clearMapMark(); // 清除访问痕迹数据，和setAccess()方法可以互换
    maze.printMaze(); // 打印带结果的迷宫地图

    system("pause");
    return 0;
}
