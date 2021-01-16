/**
 * 迷宫类文件
 * 包含通路求解和迷宫简单的美化输出方法
 * 
 * 迷宫数据说明：
 * >0 用1表示墙，,2表示暂时不可访问，用作访问标记 其余值保留
 * =0 表示通路
 * <0 按 -1,-2,-3,-4 分别表示通路的上下左右
 * 
*/

#pragma once
#ifndef MAZE
#define MAZE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <stack>
#include <queue>
#include <list>

using namespace std;

#define MaxRank 256

struct position{
    int row; // 坐标的行引索(x)
    int column; // 坐标的列引索(y)
    int passable; // （附加字段）指示下一个可通行方向
};

typedef list<position> oneway;

class Maze{
private:
    int mazeMatrix[MaxRank][MaxRank] = {0};
    int mazeRank = 0;
    position entrance = {0,0,-2};
    position exit;
    // 存储求解结果, 暂时只可存储一个解
    list<position>solution;
    // 存储求解结果集，可存储多个解
    list<oneway> solutions;
    // list<list<position>>solutions;  // 效果相同
    // 求解辅助数据
    stack<position> middleStack;
    // position current;
protected:
public:
    Maze * createMaze(int matrix[][MaxRank], int rank, position entrance, position exit){
        if(rank>MaxRank){
            cout<<"溢出！"<<endl;
            return NULL;
        }
        this->mazeRank = rank;
        for(int i=0;i<rank;i++){
            for(int j=0;j<rank;j++){
                this->mazeMatrix[i][j] = matrix[i][j];
            }
        }
        this->entrance = entrance;
        this->exit = exit;
        return this;
    }

    // 清空`solution`中的答案存储
    Maze * clearAccess(){
        this->solution.clear();
        return this;
    }
    // 清空`solutions`中的第solutionIndex个数据存储，传入-1 时全部清空
    Maze * clearAccess(int solutionIndex){
        if(solutionIndex == -1){
            this->solutions.clear();
            return this;
        }
        int index = 0;
        for(auto && i: this->solutions){
            if(index==solutionIndex){
                i.clear();
            }
            index++;
        }
        return this;
    }

    // 写入solution中的数据到迷宫map中 以供打印
    Maze * setAccess(){
        for(auto && i : this->solution){
            this->mazeMatrix[i.row][i.column] = i.passable;
        }
        return this;
    }
    // 写入solution中的第solutionIndex个引索位置的元素到迷宫map中供打印
    Maze *setAccess(int solutionIndex){
        int index = 0;
        for(auto && i : this->solutions){
            if(index == solutionIndex){
                for(auto && j : i){
                    this->mazeMatrix[j.row][j.column] = j.passable;
                }
                break;
            }
            index++;
        }
        return this;
    }

    // 返回solutions的长度 即 通路解的个数
    int solutionsLength(){
        return this->solutions.size();
    }

    // 遍历输出一条可通行的迷宫通路 并 存储供
    Maze * findSolution(bool printEachSolution = false){
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
                return this;
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
        return this;
    }


    // 调用此函数会覆盖`solutions`中的数据
    // 用深度优先遍历 查找迷宫的所有解 存储至`solutions`中
    // 传递的参数需要是起点的position结构体
    Maze * DeepFirstSearch_solution(position current, int counts = 0){
        if(counts==0){
            this->clearAccess(-1); // 万万不可！！ 先清空solutions中的所有数据，否则递归中记录不了结果
            // 首次执行先把起点压入栈中
            this->middleStack.push(current);
        }

        // 成功找到一条通路
        if(current.row==this->exit.row && current.column==this->exit.column){
            int size = this->middleStack.size();
            list<position> one_solution;
            for(int i=0;i<size;i++){
                // 栈middleStack中由栈顶到栈底是终点到起点的顺序
                one_solution.push_front(this->middleStack.top() ); // 头插法 倒序存储 栈中元素
                this->middleStack.pop(); // 一边出栈一个元素
            }
            // 此时栈空了，重复利用栈，再遍历list把数据存储进去供下次回溯
            for(auto && i : one_solution){
                // 遍历完成后栈还原 栈middleStack中由栈顶到栈底是终点到起点的顺序
                this->middleStack.push(i);
            }
            // 同时将当前访问点标记清除，与整体保持一致
            this->mazeMatrix[current.row][current.column] = 0;
            current.passable = 0; // current周围的通路设为 未知状态
            this->solutions.push_back(one_solution);
            return this;
        }
        
        position nextOne;
        // bool haveFind = false; // 标记是否找到了有效相邻通路
        
        // 查找current的相邻且为通路的宫格，需要对所有满足条件的宫格进行搜索，所以if并列代替单个选择的if...else if...else结构
        // 上方
        if(current.row - 1 > 0 && this->mazeMatrix[current.row - 1][current.column]==0){
            current.passable = -1;
            this->mazeMatrix[current.row - 1][current.column] = 2; // 下一步方向设置访问标记，置为特殊不可访问状态
            nextOne = {current.row - 1, current.column, -1};  // 获取下一步方向的宫格数据
            this->middleStack.push(nextOne); // 下一步的宫格数据入栈

            // 访问成功的情况需要 再次递归 完成后 退栈还原
            this->DeepFirstSearch_solution(nextOne, ++counts); // 递归驱动，访问下一步方向
            this->middleStack.pop(); // 递归完毕，该方向通路已经探索完成 退栈供查找其他通路时访问
            // haveFind = true;
        }
        // 右方
        if(current.column +1 < this->mazeRank && this->mazeMatrix[current.row][current.column+1]==0){
            current.passable = -2;
            this->mazeMatrix[current.row][current.column+1] = 2;
            nextOne = {current.row, current.column+1, -2};
            this->middleStack.push(nextOne);

            this->DeepFirstSearch_solution(nextOne, ++counts);
            this->middleStack.pop();
            // haveFind = true;
        }
        
        // 下方
        if(current.row + 1 < this->mazeRank && this->mazeMatrix[current.row+1][current.column]==0){
            current.passable = -3;
            this->mazeMatrix[current.row+1][current.column] = 2;
            nextOne = {current.row+1, current.column, -3};
            this->middleStack.push(nextOne);

            this->DeepFirstSearch_solution(nextOne, ++counts);
            this->middleStack.pop();
            // haveFind = true;
        }
        // 左方
        if(current.column -1 > 0 && this->mazeMatrix[current.row][current.column - 1]==0){
            current.passable = -4;
            this->mazeMatrix[current.row][current.column - 1] 2= ;
            nextOne = {current.row, current.column - 1, -4};
            this->middleStack.push(nextOne);

            this->DeepFirstSearch_solution(nextOne, ++counts);
            this->middleStack.pop();
            // haveFind = true;
        }
        // if(!haveFind) return this; // 惨痛教训：这里不能做判断，每轮不管是否寻找到 都应该清除标记状态
        //否则在回溯时图连通状态下回溯重合会导致部分结果丢失（通常是最开始的答案分支）所以havefind变量没用

        // 不能立即还原状态，应该每轮还原一次即可
        this->mazeMatrix[current.row][current.column] = 0; // 还原当前宫格的普通通路状态
        current.passable = 0; // 下一步通路方向设为 未知状态
        return this;
    }

    // 最短路径求解,广度优先遍历,可能有多条,就不组合输出,任意一条路径均可
    // 广度优先遍历的层次遍历特点，找到终点时的所有层中的任意挑选组合就是一条最短路径
    Maze * BreadthFirstSearch_solution(){
        this->clearAccess(-1); // 首先清空所有路径
        queue<position> middleQ;
        position current = this->entrance;
        position nextOne;
        middleQ.push(current);

        bool haveFind;
        while(!middleQ.empty()){
            current = middleQ.front();
            // 成功的标识
            if(current.row==this->exit.row && current.column==this->exit.column){
                cout<<"查找最短路径成功！"<<endl;
                return this;
            }

            haveFind = false; // 定义一个标记对象，下面是否找到相邻有用的路径
            // 查找current的相邻且为通路的宫格，需要对所有满足条件的宫格进行搜索，所以if并列代替单个选择的if...else if...else结构
            // 上方
            if(current.row - 1 > 0 && this->mazeMatrix[current.row - 1][current.column]==0){
                current.passable = -1;
                this->mazeMatrix[current.row - 1][current.column] = 2; // 访问标记，设置为特殊不可访问状态
                nextOne = {current.row - 1, current.column, -1};
                middleQ.push(nextOne); // 新的通路入栈
                haveFind = true;
            }
            // 右方
            if(current.column +1 < this->mazeRank && this->mazeMatrix[current.row][current.column+1]==0){
                current.passable = -2;
                this->mazeMatrix[current.row][current.column+1] = 2; // 访问标记，设置为特殊不可访问状态
                nextOne = {current.row, current.column+1, -2};
                middleQ.push(nextOne); // 新的通路入栈
                haveFind = true;
            }
            // 下方
            if(current.row + 1 < this->mazeRank && this->mazeMatrix[current.row+1][current.column]==0){
                current.passable = -3;
                this->mazeMatrix[current.row+1][current.column] = 2; // 访问标记，设置为特殊不可访问状态
                nextOne = {current.row+1, current.column, -3};
                middleQ.push(nextOne); // 新的通路入栈
                haveFind = true;
            }
            // 左方
            if(current.column -1 > 0 && this->mazeMatrix[current.row][current.column - 1]==0){
                current.passable = -4;
                this->mazeMatrix[current.row][current.column - 1] = 2; // 访问标记，设置为特殊不可访问状态
                nextOne = {current.row, current.column - 1, -4};
                middleQ.push(nextOne); // 新的通路入栈
                haveFind = true;
            }
            // 存储访问的路径至`solution`中，一层只记录一次即可，最后即是最短路径
            if(haveFind) this->solution.push_back(middleQ.front() );
            middleQ.pop();
        }
        cout<<"查找最短通路失败，迷宫无解！"<<endl;
        return this;
    }

    // 清除地图数据中的通路标记
    Maze * clearSolution(){
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
    Maze * clearMapMark(){
        for(int i=0;i<this->mazeRank;i++){
            for(int j=0;j<this->mazeRank;j++){
                // 访问标记的暂时不可访问 格式化为通路状态
                if(this->mazeMatrix[i][j]==2)
                    this->mazeMatrix[i][j] = 0;
            }
        }
        return this;
    }

    // 美化输出迷宫（包含输出通路方法）
    Maze * printMaze(){
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
                        //     cout<<"←";
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
        return this;
    }
    
    // 打印迷宫元数据
    Maze * printMazeMeta(){
    cout<<endl;
    for(int i=0;i<this->mazeRank;i++){
        for(int j=0;j<this->mazeRank;j++){
            cout<<this->mazeMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    return this;
}
    
    // 构造函数 and 析构函数
    Maze(int matrix[][MaxRank], int rank, position entrance, position exit){
        this->createMaze(matrix, rank, entrance, exit);
    }
    ~Maze(){}
};
#endif