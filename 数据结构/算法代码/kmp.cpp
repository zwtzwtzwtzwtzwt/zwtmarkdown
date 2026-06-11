#include <iostream>
#include <string>

using namespace std;

const int N = 100010;
// 避免和标准库的 std::next 冲突，命名为 next_arr
int next_arr[N]; 

// 教材版求 next 数组的函数
// 传入的 T 必须是 1 开始的下标 (例如 T = " abaabc")
void get_next(const string& T) {
    int i = 1;
    int j = 0;
    
    // 教材规定：第 1 个字符的 next 值为 0
    next_arr[1] = 0; 
    
    // T.length() - 1 是模式串的真实长度
    int m = T.length() - 1; 
    
    // 注意：这里 i 取不到 m，因为里面有一句 ++i，刚好能求出 next_arr[m]
    while (i < m) {
        // 条件 1: j == 0 说明当前没有任何匹配的前后缀，强制双指针齐步往前走
        // 条件 2: T[i] == T[j] 说明当前字符匹配成功，也一起往前走
        if (j == 0 || T[i] == T[j]) {
            ++i;
            ++j;
            next_arr[i] = j;
        } else {
            // 匹配失败时，j 进行回退
            j = next_arr[j];
        }
    }
}

int main() {
    string pattern = "abaabc";
    
    // 技巧：为了完全对齐教材中“字符串下标从 1 开始”的设定，在头部拼接一个空格
    string T = " " + pattern; 
    
    // 1. 计算 next 数组
    get_next(T);
    
    // 2. 打印结果进行验证
    cout << "模式串: " << pattern << endl;
    cout << "next数组: ";
    for (int k = 1; k <= pattern.length(); k++) {
        cout << next_arr[k] << " ";
    }
    cout << endl;
    
    return 0;
}