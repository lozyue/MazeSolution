/**
 * 迷宫 源文件
 * 第一版实现（使用了类实现）
 * 
 * 迷宫数据说明：
 * >0 用1表示墙，,2表示暂时不可访问，用作访问标记 其余值保留
 * =0 表示通路
 * <0 按 -1,-2,-3,-4 分别表示通路的上下左右
 * 
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <stack>
#include <list>

using namespace std;

#define MaxRank 256

struct position{
    int row; // 坐标的行引索(x)
    int column; // 坐标的列引索(y)
    int passable; // （附加字段）指示下一个可通行方向
};

// 迷宫类
class Maze{
private:
    // 成员数据
    int mazeMatrix[MaxRank][MaxRank] = {0};
    int mazeRank = 0;
    position entrance = {0,0,-2};
    position exit;
    // 存储求解结果, 暂时只可存储一个解
    list<position>solution;

public:
    void createMaze(int matrix[][MaxRank], int rank, position entrance, position exit){
        if(rank>MaxRank){
            cout<<"溢出！"<<endl;
            return;
        }
        this->mazeRank = rank;
        for(int i=0;i<rank;i++){
            for(int j=0;j<rank;j++){
                this->mazeMatrix[i][j] = matrix[i][j];
            }
        }
        this->entrance = entrance;
        this->exit = exit;
        return;
    }

    // 清空`solution`中的答案存储
    void clearAccess(){
        this->solution.clear();
        return;
    }

    // 写入solution中的数据到迷宫map中 以供打印
    void setAccess(){
        for(auto && i : this->solution){
            this->mazeMatrix[i.row][i.column] = i.passable;
        }
        return;
    }

    // 遍历输出一条可通行的迷宫通路 并 存储供
    void findSolution(bool printEachSolution = false){
        this->clearAccess();
        stack<position>middleS;
        middleS.push(this->entrance);
        while(!middleS.empty()){
            position current = middleS.top();
            // middleS.pop(); // 出栈一个元素
            // 找到终点
            if(current.row==this->exit.row && current.column==this->exit.column){
                cout<<"查找通路成功，解谜完毕"<<endl;
                // 将当前的通路数据保存到solution中(使用头插法可以避免翻转)
                position res;
                int size = middleS.size();
                for(int i = 0;i<size;i++){
                    res = middleS.top();
                    this->solution.push_front(res); // 头插入结果栈的栈顶元素
                    this->mazeMatrix[res.row][res.column] = 0; // 清除访问标记
                    middleS.pop(); // 退栈
                }
                return;
            }
            // 查找current的相邻通路方块
            // 上方
            if(current.row - 1 > 0 && this->mazeMatrix[current.row - 1][current.column]==0){
                current.passable = -1;
                this->mazeMatrix[current.row - 1][current.column] = 2; // 访问标记，设置为特殊不可访问状态
                position nextOne = {current.row - 1, current.column, -1};
                middleS.push(nextOne); // 新的通路入栈
            // 右方
            }else if(current.column +1 < this->mazeRank && this->mazeMatrix[current.row][current.column+1]==0){
                current.passable = -2;
                this->mazeMatrix[current.row][current.column+1] = 2; // 访问标记，设置为特殊不可访问状态
                position nextOne = {current.row, current.column+1, -2};
                middleS.push(nextOne); // 新的通路入栈
            // 下方
            }else if(current.row + 1 < this->mazeRank && this->mazeMatrix[current.row+1][current.column]==0){
                current.passable = -3;
                this->mazeMatrix[current.row+1][current.column] = 2; // 访问标记，设置为特殊不可访问状态
                position nextOne = {current.row+1, current.column, -3};
                middleS.push(nextOne); // 新的通路入栈
            // 左方
            }else if(current.column -1 > 0 && this->mazeMatrix[current.row][current.column - 1]==0){
                current.passable = -4;
                this->mazeMatrix[current.row][current.column - 1] = 2; // 访问标记，设置为特殊不可访问状态
                position nextOne = {current.row, current.column - 1, -4};
                middleS.push(nextOne); // 新的通路入栈
            // 对current的查找失败，退栈
            }else{
                middleS.pop();
                // this->mazeMatrix[current.row][current.column] = 0; // 还原普通通路状态,错误，此时不应该还原状态
                // current.passable = 0; // current周围的通路设为 未知状态
            }
        }
        cout<<"迷宫无解！"<<endl;
        return;
    }

    // 清除地图数据中的通路标记
    void clearSolution(){
        for(int i=0;i<this->mazeRank;i++){
            for(int j=0;j<this->mazeRank;j++){
                // 小于零的值格式化为0 通路
                if(this->mazeMatrix[i][j] < 0){
                    this->mazeMatrix[i][j] = 0;
                // 其余大于零的值 格式化为1 墙
                }else if(this->mazeMatrix[i][j] > 0){
                    this->mazeMatrix[i][j] = 1;
                }
            }
        }
    }

    // 清除求解通路时产生的 暂时不可访问标记,在调用搜索通路后的打印迷宫前调用
    void clearMapMark(){
        for(int i=0;i<this->mazeRank;i++){
            for(int j=0;j<this->mazeRank;j++){
                // 访问标记的暂时不可访问 格式化为通路状态
                if(this->mazeMatrix[i][j]==2)
                    this->mazeMatrix[i][j] = 0;
            }
        }
        return;
    }

    // 美化输出迷宫（包含输出通路方法）
    void printMaze(){
        system("");
        cout<<endl;
        // 出入口标记
        this->mazeMatrix[entrance.row][entrance.column] = -6;
        this->mazeMatrix[exit.row][exit.column] = -7;
        for(int i=0;i<this->mazeRank;i++){
            for(int j=0;j<this->mazeRank;j++){
                if(this->mazeMatrix[i][j]>0){
                    // 方块表示墙(长度为二)
                    cout<<"▇";
                }else if(this->mazeMatrix[i][j]<0){
                    // 通路输出表示
                    switch (this->mazeMatrix[i][j]){
                        // -1 to -4 遵循上右左下的原则
                        case -1:
                            cout<<"\033[32m"; // 上色，绿色
                            cout<<"↑";
                            break;
                        case -2:
                            cout<<"\033[32m"; // 上色，绿色
                            cout<<"→";
                            break;
                        case -3:
                            cout<<"\033[32m"; // 上色，绿色
                            cout<<"↓";
                            break;
                        case -4:
                            cout<<"\033[32m"; // 上色，绿色
                            cout<<"←";
                            break;
                            
                        // 入口标记
                        case -6:
                            cout<<"\033[5;35m"; // 上色，紫红色慢闪
                            // cout<<"✈"; // ⚘
                            cout<<"> ";
                            break;
                        // 出口标记
                        case -7:
                            cout<<"\033[31m"; // 上色，红色
                            // cout<<"⇲";
                            cout<<"⊿";
                            break;
                        default:
                            break;
                    }
                    cout<<"\033[0m"; // 颜色End
                }else{
                    // 空白表示通路(两个空格)
                    cout<<"  ";
                }
                // cout<<" ";
            }
            cout<<endl;
        }
        // 出入口标记还原
        this->mazeMatrix[entrance.row][entrance.column] = 0;
        this->mazeMatrix[exit.row][exit.column] = 0;
        return;
    }
    
    // 打印迷宫元数据
    void printMazeMeta(){
        cout<<endl;
        for(int i=0;i<this->mazeRank;i++){
            for(int j=0;j<this->mazeRank;j++){
                cout<<this->mazeMatrix[i][j]<<" ";
            }
            cout<<endl;
        }
        return;
    }
    
    // 构造函数 and 析构函数
    Maze(int matrix[][MaxRank], int rank, position entrance, position exit){
        this->createMaze(matrix, rank, entrance, exit);
    }
    ~Maze(){}
};


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
