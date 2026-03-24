#include <iostream>
using namespace std;

class Calculator {
private:
    int result;
    float value;
    
public:
    int add(int a, int b) {
        result = a + b;
        return result;
    }
    
    float divide(float x, float y) {
        if (y != 0) {
            return x / y;
        }
        return 0.0;
    }
    
    void display() {
        cout << "Result: " << result;
    }
};

int main() {
    int num1 = 10;
    int num2 = 20;
    float sum = 0.0;
    char grade = 'A';
    
    Calculator calc;
    sum = calc.add(num1, num2);
    
    if (sum > 25) {
        num1++;
    } else {
        num2--;
    }
    
    for (int i = 0; i < 5; i++) {
        num1 = num1 + 1;
    }
    
    while (num2 > 0) {
        num2--;
    }
    
    int _validID = 100;
    
    return 0;
}
