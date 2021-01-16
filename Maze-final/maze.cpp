/**
 * 迷宫 源文件
 * 
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
    // 11阶升级版迷宫
    int mazeMatrixUpgrate[][MaxRank] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1},
        {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    position entrance = { 1, 0, -2},
             exit = {4, 5, -2},
             upgrateExit = {9, 10, -2};
    
    Maze * maze = new Maze(mazeMatrix, sizeof(mazeMatrix)/sizeof(mazeMatrix[0]), entrance, exit);
    //　升级版调用切换
    // Maze * maze = new Maze(mazeMatrixUpgrate, sizeof(mazeMatrixUpgrate)/sizeof(mazeMatrixUpgrate[0]), entrance, upgrateExit);
    
    // maze->printMazeMeta();
    maze->printMaze();

    maze->findSolution()->setAccess()->clearMapMark()->printMaze()->clearSolution(); // setAccess后需要清除地图标记供下次遍历

    // 广度优先遍历寻找最短路径
    maze->BreadthFirstSearch_solution();
    maze->setAccess()->clearMapMark()->printMaze()->clearSolution();

    // 深度优先遍历 查找所有通路
    maze->DeepFirstSearch_solution(entrance);

    // 输出所有搜索到的solution
    int num = maze->solutionsLength();
    cout<<endl<<endl<<"深度搜索结果：共有"<<num<<"条通路"<<endl;
    for(int i=0;i<num;i++){
        maze->setAccess(i)->clearMapMark()->printMaze()->clearSolution();
    }
    system("pause");
    return 0;
}
